"""Real-time 3D drone viewer / simulator for the QuadCopter F450 firmware.

Modes
-----
  Live (hardware):   python drone_viewer.py --port COM3
      Reads the "$TLM" telemetry (firmware DEBUG=6, sent automatically) and
      rotates the 3D drone with the real attitude. Props are coloured by
      motor command.

  Live + control:    python drone_viewer.py --port COM3 --control
      Additionally drives the drone from the keyboard by sending
      @QC+SP / @QC+ARM / @QC+DISARM. Needs the firmware built with
      FC_VARIANT_UART_TEST. PROPELLERS OFF or frame tethered!

  Simulation:        python drone_viewer.py --sim
      No hardware: runs the same controller as the firmware (fc_model.py,
      gains from USER_Config.h) on an F450 physics model.

  Self test:         python drone_viewer.py --selftest   (no window)

Keys (control / sim)
--------------------
  SPACE arm      X disarm      W / S throttle up / down     H hover throttle (sim)
  Arrow keys     pitch / roll (held)                        A / D yaw left / right (held)
  C gyro calib   L level trim  R reset simulation           Q quit

Requirements: pip install vpython pyserial
Frames: firmware FRD/NED. On screen: north = into the screen, up = up.
"""
# ===========================================================================
# OVERVIEW
# (Kept as comments, not in the docstring above: the docstring is printed
#  by --help through argparse, and that output must not change.)
# ===========================================================================
# Purpose
#   Shows the drone in 3D in a web browser (vpython) and optionally flies it
#   from the keyboard. One rendering loop (run_viewer) works with two
#   interchangeable "data sources" that share the same interface
#   (step(sticks, dt), snapshot(), close()):
#     - SerialSource : real board over UART.
#     - SimSource    : no board, fc_model.FlightController + QuadPlant.
#
# The 4 modes
#   --port COMx            LIVE: sends @QC+DEBUG=6 so the firmware streams
#                          "$TLM,..." lines; the 3D model follows the REAL
#                          attitude, props are coloured by motor command.
#                          Read-only: the keyboard sends nothing to the board.
#   --port COMx --control  LIVE + CONTROL: also sends @QC+ARM / @QC+DISARM /
#                          @QC+CALIB / @QC+LEVEL and @QC+SP=thr,roll,pitch,yaw
#                          from the keyboard. Only accepted by firmware built
#                          with FC_VARIANT_UART_TEST. REMOVE THE PROPS or tether
#                          the frame. Throttle is capped at
#                          UART_SP_THROTTLE_MAX_US (read from USER_Config.h).
#   --sim                  SIMULATION: the same controller as the firmware
#                          (gains from USER_Config.h) flying the F450 physics
#                          model in real time. Good for getting a feel for the
#                          controls / gains without hardware.
#   --selftest             No window: checks screen orientation, keyboard
#                          mapping, a scripted sim flight and the parser.
#                          Prints "SELFTEST PASSED".
#
# Background serial thread (SerialSource._reader)
#   serial.read() blocks for up to 0.2 s; doing it in the render loop would
#   stutter the display. A daemon thread reads continuously, splits lines,
#   parses them and writes the results into self.data under self.lock; the
#   render loop only takes a copy via snapshot(). If no telemetry arrives for
#   0.5 s, "stale" is set and the HUD shows "(no telemetry)".
#
# NED -> screen coordinates (ned_to_screen)
#   vpython uses x = right, y = up, z = towards the viewer. Mapping:
#     screen x = East  = +n[1]
#     screen y = Up    = -n[2]   (NED z points down)
#     screen z = South = -n[0]   (the camera looks north = into the screen)
#   The matrix has determinant +1, i.e. it is a PROPER ROTATION, not a
#   mirror. A mirror (det -1) would show the drone with left and right
#   swapped and rotations in the wrong direction (e.g. roll right drawn as
#   roll left) -- very misleading while checking signs. The self test checks
#   this orientation.
#
# Keyboard -> setpoints (class Sticks)
#   Arrow keys = roll/pitch angle +-STICK_ANGLE_DEG while held (released =
#   back to 0 = self-levelling); up arrow = stick forward = nose DOWN
#   (pitch < 0). A/D = yaw rate +-STICK_YAW_DPS. W/S ramp the throttle at
#   THROTTLE_RATE_US_S. One-shot keys (SPACE, X, C, L, R, H) go into an
#   event queue, consumed by the source on its next step.
#
# @QC+SP rate limiting
#   A command is only sent when the (rounded) setpoint changes, plus a
#   resend once per second as a keep-alive. This avoids flooding the
#   115200-baud UART with a command every frame (60 per second).
#
# Safety
#   Q (or Ctrl+C) -> source.close(): in --control mode it ALWAYS sends
#   @QC+DISARM before closing the port, then @QC+DEBUG=0 to stop the stream.
# ===========================================================================
import argparse
import math
import sys
import threading
import time

import fc_model as fm

# Stick parameters (keyboard -> setpoints)
THROTTLE_RATE_US_S = 300.0     # W/S ramp speed
STICK_ANGLE_DEG = 15.0         # arrow keys
STICK_YAW_DPS = 90.0           # A/D
# Drawing geometry only; must match QuadPlant.ARM / MX / MY / SPIN in fc_model.
ARM_LEN = 0.159                # m, motor offset along x and y (0.225 * cos45)
MOTOR_POS_FRD = [(ARM_LEN, -ARM_LEN), (-ARM_LEN, -ARM_LEN), (ARM_LEN, ARM_LEN), (-ARM_LEN, ARM_LEN)]  # M1 FL, M2 BL, M3 FR, M4 BR
SPIN = [-1, +1, +1, -1]        # +1 CCW (M1 FL CW, M2 BL CCW, M3 FR CCW, M4 BR CW)


# --------------------------------------------------------------------------
# Math helpers (no vpython dependency, used by the self test)
# --------------------------------------------------------------------------
def rotation(roll_deg, pitch_deg, yaw_deg):
    """Body FRD -> world NED rotation matrix."""
    r, p, y = map(math.radians, (roll_deg, pitch_deg, yaw_deg))
    cr, sr, cp, sp, cy, sy = math.cos(r), math.sin(r), math.cos(p), math.sin(p), math.cos(y), math.sin(y)
    return [[cy * cp, cy * sp * sr - sy * cr, cy * sp * cr + sy * sr],
            [sy * cp, sy * sp * sr + cy * cr, sy * sp * cr - cy * sr],
            [-sp, cp * sr, cp * cr]]


def body_to_ned(R, v):
    """Multiply R (3x3) by vector v: body FRD vector -> NED."""
    return [R[i][0] * v[0] + R[i][1] * v[1] + R[i][2] * v[2] for i in range(3)]


def ned_to_screen(n):
    """NED -> screen (x = east, y = up, z = south). Proper rotation (det +1).

    See OVERVIEW: det +1 means no mirroring, so screen rotations have the
    same sense as the real ones.
    """
    return (n[1], -n[2], -n[0])


def motor_color(us):
    """1000 us -> blue, 2000 us -> red.

    Returns (r, g, b) in 0..1; green peaks mid-range (1500 us) so the
    colour goes blue -> greenish -> red.
    """
    t = max(0.0, min(1.0, (us - 1000.0) / 1000.0))
    return (t, 0.2 + 0.6 * (1 - abs(2 * t - 1)), 1 - t)


# --------------------------------------------------------------------------
# Keyboard state -> setpoints
# --------------------------------------------------------------------------
class Sticks:
    """Keyboard state -> virtual sticks (throttle, roll, pitch, yaw rate).

    held   : keys currently pressed (continuous controls: arrows, W/S, A/D).
    events : queue of one-shot actions (arm, disarm, ...), drained by the
             source through pop_events().
    """

    def __init__(self):
        self.held = set()
        self.throttle = 1000.0
        self.events = []       # one-shot actions: 'arm', 'disarm', 'calib', 'level', 'reset', 'hover', 'quit'

    def key_down(self, key):
        """Called by vpython on keydown; key is e.g. 'w', ' ', 'up', 'left'."""
        key = key.lower()
        self.held.add(key)
        action = {" ": "arm", "x": "disarm", "c": "calib", "l": "level",
                  "r": "reset", "h": "hover", "q": "quit"}.get(key)
        if action:
            self.events.append(action)

    def key_up(self, key):
        """Called by vpython on keyup."""
        self.held.discard(key.lower())

    def update(self, dt):
        """Ramp the throttle while W/S is held; keep it within 1000..2000 us."""
        if "w" in self.held:
            self.throttle += THROTTLE_RATE_US_S * dt
        if "s" in self.held:
            self.throttle -= THROTTLE_RATE_US_S * dt
        self.throttle = max(1000.0, min(2000.0, self.throttle))

    def setpoints(self):
        """Return (throttle us, roll deg, pitch deg, yaw rate deg/s) from the held keys."""
        # right arrow = roll right (right side down) = positive roll in FRD
        roll = (STICK_ANGLE_DEG if "right" in self.held else 0.0) - (STICK_ANGLE_DEG if "left" in self.held else 0.0)
        # arrow up = stick forward = nose down
        pitch = (STICK_ANGLE_DEG if "down" in self.held else 0.0) - (STICK_ANGLE_DEG if "up" in self.held else 0.0)
        # D = yaw right (nose right) = positive yaw rate
        yaw = (STICK_YAW_DPS if "d" in self.held else 0.0) - (STICK_YAW_DPS if "a" in self.held else 0.0)
        return self.throttle, roll, pitch, yaw

    def pop_events(self):
        """Return the queued one-shot events and clear the queue."""
        ev, self.events = self.events, []
        return ev


# --------------------------------------------------------------------------
# Data sources
# --------------------------------------------------------------------------
class SerialSource:
    """Reads telemetry from the board in a background thread.

    control = True -> also send keyboard commands (--control mode).
    max_thr = throttle cap for @QC+SP (UART_SP_THROTTLE_MAX_US).
    """

    def __init__(self, port, baud, control, max_thr):
        import serial  # pyserial  (imported here so --sim / --selftest work without pyserial)
        # Short timeout so the reader thread re-checks self.running often.
        self.ser = serial.Serial(port, baud, timeout=0.2)
        self.control = control
        self.max_thr = max_thr
        self.lock = threading.Lock()   # guards data / last_rx / last_line (shared with the reader thread)
        self.data = {"state": "NO DATA", "roll": 0.0, "pitch": 0.0, "yaw": 0.0,
                     "motors": [1000] * 4, "throttle": 1000.0, "variant": None}
        self.last_rx = 0.0             # time of the last valid telemetry line
        self.last_line = ""            # last non-telemetry line (log / reply), shown in the HUD
        self.running = True
        self._last_sp = None           # last @QC+SP sent (for rate limiting)
        self._last_sp_time = 0.0
        self.send("@QC+DEBUG=6")       # ask the firmware to stream $TLM lines
        # daemon=True: the thread does not keep Python alive after main() returns.
        threading.Thread(target=self._reader, daemon=True).start()

    def send(self, cmd):
        """Send one command line to the firmware (terminated with CR+LF)."""
        self.ser.write((cmd + "\r\n").encode())

    def _reader(self):
        """Background thread: read bytes, split into lines, update self.data."""
        buf = b""
        while self.running:
            try:
                buf += self.ser.read(256)
            except Exception:
                break          # port closed / unplugged: end the thread
            while b"\n" in buf:
                raw, buf = buf.split(b"\n", 1)
                line = raw.decode(errors="ignore").strip()
                t = fm.parse_telemetry(line)
                with self.lock:
                    if t:
                        # Only overwrite fields the line actually carries
                        # (the legacy line has no motors/state -> keep old values).
                        for k, v in t.items():
                            if v is not None:
                                self.data[k] = v
                        self.last_rx = time.time()
                    elif line:
                        self.last_line = line

    def step(self, sticks, dt):
        """Called every frame: send events / setpoints to the board (control mode only)."""
        for ev in sticks.pop_events():
            if not self.control:
                continue       # view-only mode: drop the events, send nothing
            cmd = {"arm": "@QC+ARM", "disarm": "@QC+DISARM", "calib": "@QC+CALIB",
                   "level": "@QC+LEVEL"}.get(ev)
            if cmd:
                self.send(cmd)
        if self.control:
            thr, r, p, y = sticks.setpoints()
            thr = min(thr, self.max_thr)   # bench-test throttle cap
            # Round before comparing so tiny changes do not count as "changed".
            sp = (round(thr), round(r, 1), round(p, 1), round(y, 1))
            now = time.time()
            # Send only on change, or every 1 s as a keep-alive.
            if sp != self._last_sp or now - self._last_sp_time > 1.0:
                self.send("@QC+SP=%d,%.1f,%.1f,%.1f" % sp)
                self._last_sp, self._last_sp_time = sp, now

    def snapshot(self):
        """Return a copy of the current data, taken under the lock."""
        with self.lock:
            d = dict(self.data)
            d["motors"] = list(self.data["motors"])
            d["stale"] = (time.time() - self.last_rx) > 0.5
            d["pos"] = (0.0, 0.0, -0.6)     # fixed position in live mode
            d["info"] = self.last_line
        return d

    def close(self):
        """Shut down safely: DISARM (control mode), stop the stream and the thread, close the port."""
        if self.control:
            self.send("@QC+DISARM")
        self.send("@QC+DEBUG=0")
        self.running = False
        time.sleep(0.1)               # give the reader thread time to see running=False
        self.ser.close()


class SimSource:
    """Firmware controller (fc_model) + F450 physics, real time."""

    def __init__(self, cfg):
        self.cfg = cfg
        # Controller period = the firmware's (CTRL_LOOP_PERIOD_MS).
        self.dt = cfg["CTRL_LOOP_PERIOD_MS"] / 1000.0
        self.reset()

    def reset(self):
        """New controller + new physics model (drone on the ground, disarmed)."""
        self.fc = fm.FlightController(self.cfg)
        self.plant = fm.QuadPlant()
        # One disarmed step initialises the attitude estimate (needed to arm).
        self.fc.step(*self.plant.sensors(), self.dt, 1000, 0, 0, 0)
        self.acc_time = 0.0            # real time not yet simulated
        self.n = 0
        self.msg = "Simulation reset"

    def step(self, sticks, dt):
        """Handle events, then run as many control periods as real time has elapsed."""
        for ev in sticks.pop_events():
            if ev == "arm":
                self.msg = self.fc.arm(sticks.throttle)
            elif ev == "disarm":
                self.fc.disarm()
                self.msg = "Disarmed"
            elif ev == "reset":
                self.reset()
                sticks.throttle = 1000.0
            elif ev == "hover":
                sticks.throttle = self.plant.hover_us()
            elif ev == "level":
                self.msg = "(level trim: hardware only)"
            elif ev == "calib":
                self.msg = "(gyro calib: hardware only)"
        thr, r, p, y = sticks.setpoints()
        # Fixed-step accumulator: the controller always runs at self.dt like
        # the firmware, whatever the frame rate. min(dt, 0.1) avoids a long
        # catch-up burst after the window stalls (e.g. while being dragged).
        self.acc_time += min(dt, 0.1)
        while self.acc_time >= self.dt:
            self.acc_time -= self.dt
            self.n += 1
            g, a = self.plant.sensors(noise=0.2, n=self.n)
            m = self.fc.step(g, a, self.dt, thr, r, p, y)
            for _ in range(4):        # 4 physics sub-steps per control period
                self.plant.step(m, self.dt / 4)
        if self.fc.state == "FAILSAFE" and "crash" not in self.msg:
            self.msg = "FAILSAFE (tilt > limit) - press R to reset"

    def snapshot(self):
        """Current state for drawing. Shows the TRUE attitude of the physics
        model; "est" is the controller's own estimate (shown in the HUD)."""
        pl = self.plant
        return {"state": self.fc.state, "roll": math.degrees(pl.roll), "pitch": math.degrees(pl.pitch),
                "yaw": math.degrees(pl.yaw), "motors": list(self.fc.motors),
                "throttle": None, "variant": None, "stale": False,
                "pos": tuple(pl.pos), "info": self.msg,
                "est": (self.fc.att.roll, self.fc.att.pitch)}

    def close(self):
        """Nothing to release in simulation."""
        pass


# --------------------------------------------------------------------------
# 3D view (vpython)
# --------------------------------------------------------------------------
def run_viewer(source, sticks, title, show_trail):
    """Open the 3D scene in the browser and run the display loop (~60 fps) until Q.

    Each frame: update sticks -> source.step() -> snapshot() -> transform
    every part body FRD -> NED -> screen -> refresh the HUD text.
    """
    # Imported here so the self test does not need vpython.
    from vpython import (box, canvas, color, cone, curve, cylinder, rate, sphere,
                         vector, wtext)

    def v(t):
        """tuple (x, y, z) -> vpython vector."""
        return vector(t[0], t[1], t[2])

    scene = canvas(title=title, width=1000, height=640, background=vector(0.62, 0.78, 0.92))
    scene.up = vector(0, 1, 0)                # screen y = up (see ned_to_screen)
    scene.forward = vector(0, -0.35, -1)     # look north, slightly down
    scene.range = 1.0

    # Ground + grid
    box(pos=vector(0, -0.01, 0), size=vector(40, 0.02, 40), color=vector(0.35, 0.55, 0.3))
    for i in range(-20, 21, 1):
        curve(pos=[vector(i, 0.001, -20), vector(i, 0.001, 20)], color=vector(0.3, 0.45, 0.28))
        curve(pos=[vector(-20, 0.001, i), vector(20, 0.001, i)], color=vector(0.3, 0.45, 0.28))
    # North marker
    # Screen -z = North (into the screen).
    cone(pos=vector(0, 0.002, -3), axis=vector(0, 0, -0.6), radius=0.2, color=color.red)

    body = box(size=vector(0.16, 0.05, 0.16), color=vector(0.15, 0.15, 0.15))
    nose = cone(radius=0.03, color=color.red)
    # Front arms (M1 FL, M3 FR) red, rear arms (M2 BL, M4 BR) white -> the
    # nose is easy to see. Index = motor number - 1 (see MOTOR_POS_FRD).
    arms = [cylinder(radius=0.012, color=(color.red if MOTOR_POS_FRD[i][0] > 0 else color.white))
            for i in range(4)]
    motors = [cylinder(radius=0.022, color=vector(0.3, 0.3, 0.3)) for _ in range(4)]
    props = [cylinder(radius=0.12, opacity=0.55) for _ in range(4)]
    shadow = cylinder(radius=0.25, axis=vector(0, 0.002, 0), color=vector(0.2, 0.3, 0.2), opacity=0.5)
    trail_ball = sphere(radius=0.001, make_trail=show_trail, trail_radius=0.008,
                        trail_color=color.orange, retain=600)

    scene.append_to_caption("\n")
    hud = wtext(text="")
    scene.append_to_caption(
        "\n\nKeys: SPACE arm | X disarm | W/S throttle | arrows pitch/roll | A/D yaw | "
        "H hover (sim) | C calib | L level | R reset (sim) | Q quit\n"
        "Click the 3D view first so it receives the keyboard.")

    # Browser key events -> Sticks (click the 3D view first so it has keyboard focus).
    scene.bind("keydown", lambda e: sticks.key_down(e.key))
    scene.bind("keyup", lambda e: sticks.key_up(e.key))

    last = time.time()
    while True:
        rate(60)
        now = time.time()
        dt, last = now - last, now
        sticks.update(dt)
        if "q" in sticks.held:
            break                     # leave the loop -> source.close() below (DISARM)
        source.step(sticks, dt)
        d = source.snapshot()

        R = rotation(d["roll"], d["pitch"], d["yaw"])
        c = v(ned_to_screen(d["pos"]))   # drone centre on screen

        def world(b):  # body FRD offset -> screen vector
            return v(ned_to_screen(body_to_ned(R, b)))

        # Nose direction (body +x) and "up" (body -z, because FRD z points down).
        fwd, up = world((1, 0, 0)), world((0, 0, -1))
        body.pos, body.axis, body.up = c, fwd * 0.16, up
        nose.pos, nose.axis = c + fwd * 0.08, fwd * 0.07
        for i, (mx, my) in enumerate(MOTOR_POS_FRD):
            mp = world((mx, my, 0))
            arms[i].pos, arms[i].axis = c, mp
            motors[i].pos, motors[i].axis = c + mp, up * 0.03
            props[i].pos, props[i].axis = c + mp + up * 0.035, up * 0.004
            us = d["motors"][i] if d["motors"] else 1000
            props[i].color = v(motor_color(us))
        shadow.pos = vector(c.x, 0.001, c.z)   # shadow on the ground below the drone (height cue)
        trail_ball.pos = c
        scene.center = c                       # camera follows the drone

        m = d["motors"] or [0, 0, 0, 0]
        alt = -d["pos"][2]
        lines = [
            "<b>%s</b>%s" % (d["state"], "  <span style='color:red'>(no telemetry)</span>" if d["stale"] else ""),
            "roll %+6.1f  pitch %+6.1f  yaw %+7.1f deg" % (d["roll"], d["pitch"], d["yaw"]),
            "M1 %4d  M2 %4d  M3 %4d  M4 %4d us" % tuple(m),
            "stick: throttle %4.0f  roll %+5.1f  pitch %+5.1f  yaw %+6.1f" % sticks.setpoints(),
        ]
        if "est" in d:
            lines.append("altitude %.2f m   FC estimate roll %+5.1f pitch %+5.1f" % (alt, d["est"][0], d["est"][1]))
        if d.get("info"):
            lines.append(str(d["info"])[:90])
        hud.text = "<br>".join(lines)

    source.close()


# --------------------------------------------------------------------------
# Self test (headless)
# --------------------------------------------------------------------------
def selftest(cfg):
    """Headless self test (no vpython / board needed). Returns 0 if everything passes.

    Checks: (1) screen orientation for roll / pitch / yaw, (2) keyboard
    mapping -> setpoint signs, (3) a scripted SimSource flight (arm, take
    off, right arrow -> roll right and drift east, disarm), (4) the $TLM parser.
    """
    ok = True

    def check(cond, msg):
        nonlocal ok
        print(("  ok  : " if cond else "  FAIL: ") + msg)
        ok = ok and cond

    def scr(roll, pitch, yaw, b):
        """Where body vector b points on screen for the given attitude."""
        return ned_to_screen(body_to_ned(rotation(roll, pitch, yaw), b))

    print("Screen orientation")
    n = scr(0, 0, 0, (1, 0, 0))
    check(abs(n[2] + 1) < 1e-9, "level: nose points into the screen (north)")
    r = scr(30, 0, 0, (0, 1, 0))
    check(r[1] < -0.4, "roll +30: right side goes down (y=%.2f)" % r[1])
    n = scr(0, 20, 0, (1, 0, 0))
    check(n[1] > 0.3, "pitch +20: nose goes up (y=%.2f)" % n[1])
    n = scr(0, 0, 90, (1, 0, 0))
    check(n[0] > 0.99, "yaw +90: nose points east / right (x=%.2f)" % n[0])

    print("Keyboard mapping")
    s = Sticks()
    s.key_down("up"); s.key_down("right"); s.key_down("d")
    _, r, p, y = s.setpoints()
    check(p < 0 and r > 0 and y > 0, "up=nose down, right=roll right, D=yaw right")
    s.key_down("w"); s.update(1.0)       # 1 s of W -> 1000 + 300 = 1300 us
    check(abs(s.throttle - 1300) < 1e-6, "W ramps throttle %.0f" % s.throttle)

    print("Simulation source (scripted flight)")
    sim = SimSource(cfg)
    st = Sticks()
    st.key_down(" ")
    sim.step(st, 0.02)
    check(sim.fc.state == "ARMED", "arm: " + sim.msg)
    st.throttle = sim.plant.hover_us() + 60
    for _ in range(int(2.0 / 0.02)):
        sim.step(st, 0.02)
    alt = -sim.plant.pos[2]
    check(alt > 0.3, "takes off (altitude %.2f m)" % alt)
    st.throttle = sim.plant.hover_us()
    st.key_down("right")
    for _ in range(int(2.0 / 0.02)):
        sim.step(st, 0.02)
    roll = math.degrees(sim.plant.roll)
    check(abs(roll - STICK_ANGLE_DEG) < 2 and sim.plant.pos[1] > 0.3,
          "right arrow: roll %.1f deg, drifts east %.2f m" % (roll, sim.plant.pos[1]))
    st.key_up("right")
    st.key_down("x")
    sim.step(st, 0.02)
    check(sim.fc.state == "DISARMED", "disarm")

    print("Telemetry parser")
    # A leading '\r' (left over from CR+LF) must not break the parse.
    t = fm.parse_telemetry("\r$TLM,FAILSAFE,1.00,-2.00,3.00,1000,1000,1000,1000,1000,1")
    check(t is not None and t["state"] == "FAILSAFE" and t["variant"] == 1, "parse $TLM")

    print("\nSELFTEST " + ("PASSED" if ok else "FAILED"))
    return 0 if ok else 1


def main():
    """Parse arguments, pick the data source for the mode, and run the viewer."""
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--port", help="serial port of the board, e.g. COM3")
    ap.add_argument("--baud", type=int, default=115200)
    ap.add_argument("--control", action="store_true",
                    help="send keyboard setpoints (firmware FC_VARIANT_UART_TEST only)")
    ap.add_argument("--sim", action="store_true", help="offline simulation, no hardware")
    ap.add_argument("--config", default=fm.DEFAULT_CONFIG, help="path to USER_Config.h")
    ap.add_argument("--trail", action="store_true", help="draw the flight path")
    ap.add_argument("--selftest", action="store_true", help="headless self test")
    args = ap.parse_args()

    cfg = fm.load_config(args.config)   # constants from the firmware header
    if args.selftest:
        return selftest(cfg)

    sticks = Sticks()
    if args.sim:
        source = SimSource(cfg)
        title = "QuadCopter F450 - SIMULATION (firmware controller + physics)"
    elif args.port:
        source = SerialSource(args.port, args.baud, args.control, cfg["UART_SP_THROTTLE_MAX_US"])
        title = "QuadCopter F450 - LIVE %s%s" % (args.port, " + CONTROL" if args.control else "")
        if args.control:
            print("CONTROL MODE: propellers off or frame tethered! Q quits and disarms.")
    else:
        ap.print_help()
        return 2

    try:
        run_viewer(source, sticks, title, args.trail or args.sim)
    except KeyboardInterrupt:
        source.close()                  # Ctrl+C also DISARMs (control mode)
    return 0


if __name__ == "__main__":
    sys.exit(main())
