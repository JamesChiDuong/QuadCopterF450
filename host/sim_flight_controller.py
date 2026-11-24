"""Closed-loop regression test of the flight controller (no hardware needed).

Uses fc_model.py: a port of the firmware controller with the constants read
from USER_Config.h, against an F450 physics model whose torques come from the
motor geometry (independent of the mixer table), so wrong signs diverge.

Run after changing gains / mixer signs in USER_Config.h:
    python sim_flight_controller.py [path/to/USER_Config.h]

A pass means "signs and gains are sane on an estimated model", not
"it will fly perfectly".

===========================================================================
OVERVIEW
===========================================================================
Purpose
    Automatic regression test, run on the PC, of the firmware controller.
    Run it every time you change a PID gain, a limit or a mixer sign in
    USER_Config.h, BEFORE flashing the board. The last line prints
    "ALL PASSED" or "FAILED (n)"; the exit code is 0 / 1, so it can be used
    in a script.

How it works
    fc_model.FlightController (line-by-line port of the firmware, with gains
    from USER_Config.h) is closed-loop with fc_model.QuadPlant (F450 physics).
    Each control period (CTRL_LOOP_PERIOD_MS) the controller reads simulated
    sensors and outputs 4 motor commands; the physics then runs 10 smaller
    sub-steps.

The tests and what a pass proves
  1. Unit checks
     - "+roll -> left motors faster": the MIX_*_ROLL signs make a positive
       roll command (right side down) speed up the LEFT motors (M1, M2).
     - "+pitch -> front motors faster": positive pitch (nose up) speeds up
       the FRONT motors (M1, M3).
     - "saturation keeps differential": when a motor hits 2000 us the mixer
       shifts all motors together and keeps the 400 us difference (attitude
       authority is not lost).
     - "PID P+I first step": basic PID arithmetic (2*1 + 1*1*0.1 = 2.1).
  2. Attitude control (fixed_position = gimbal rig, rotation only)
     - arming with low throttle succeeds.
     - recovers from 10 / -8 deg to level (error < 1 deg): the full loop
       (attitude estimate + angle P + rate PID + mixer + motors) has the right
       signs and is stable, even with gyro bias and noise.
     - tracks roll 20 / pitch -15: follows a setpoint, not just holds level.
     - yaw rate 90 dps: yaw sign / gain correct (MIX_*_YAW matches SPIN).
     - back to level after the manoeuvres: no stuck integrator.
  3. Take-off and hover (full 6-DOF model, with gravity and ground)
     - climbs with throttle hover + 60 us: thrust and ground release work.
     - stays level near hover: low drift (< 1 m) -> no hidden tilt.
     - nose down 10 deg -> moves forward (+x / North): pitch sign is
       consistent with the FRD/NED frames (stick forward = fly forward).
  4. Negative control (proves the test itself is meaningful)
     - Deliberately FLIPS every MIX_Mx_ROLL sign and checks that the drone
       DIVERGES (roll peak > 45 deg, or the controller drops into FAILSAFE).
     - Why it matters: if the simulation still stayed stable with a wrong
       mixer, every test above would be worthless (e.g. if torques were
       computed from the mixer table itself, wrong signs would "cancel out").
       Because QuadPlant computes torques from motor geometry (r x F), a
       wrong sign really makes it flip. This test guarantees the simulation
       CAN detect sign errors.
  5. Telemetry parser
     - parses both the $TLM line (DEBUG=6) and the legacy line (DEBUG=1).

Limits
    The model parameters are estimates. Passing = signs and gains are
    sensible; it does NOT replace step-by-step bench testing (props off,
    then tethered).
"""
import math
import sys

import fc_model as fm

fails = 0   # number of failed checks (global counter)


def check(cond, msg):
    """Print 'ok' / 'FAIL' for one condition and count failures."""
    global fails
    print(("  ok  : " if cond else "  FAIL: ") + msg)
    if not cond:
        fails += 1


def run(fc, pl, secs, thr, sr, sp, sy, bias, dt, counter):
    """Fly 'secs' seconds; return max abs errors during the last second.

    fc = controller, pl = physics model, thr = throttle [us],
    sr / sp = roll / pitch setpoints [deg], sy = yaw-rate setpoint [deg/s].
    counter is a 1-element list (mutable) holding the global step counter,
    used to generate continuous sensor "noise" across calls.
    Returns (roll error, pitch error, yaw-rate error, peak |roll| over the
    whole run). Errors are measured on the TRUE state of the physics model,
    not on the controller's estimate.
    """
    steps = int(secs / dt)
    er = ep = ey = peak = 0.0
    for k in range(steps):
        counter[0] += 1
        g, a = pl.sensors(bias, noise=0.3, n=counter[0])
        m = fc.step(g, a, dt, thr, sr, sp, sy)
        # Physics runs 10 sub-steps per control period (more accurate
        # integration; the motor commands are held constant, as in reality).
        for _ in range(10):
            pl.step(m, dt / 10)
        peak = max(peak, abs(math.degrees(pl.roll)))
        # Only measure errors over the last second (after settling).
        if k >= steps - int(1 / dt):
            er = max(er, abs(math.degrees(pl.roll) - sr))
            ep = max(ep, abs(math.degrees(pl.pitch) - sp))
            ey = max(ey, abs(math.degrees(pl.r) - sy))
    return er, ep, ey, peak


def main():
    """Run every test group; return the exit code (0 = all passed)."""
    cfg = fm.load_config(sys.argv[1] if len(sys.argv) > 1 else fm.DEFAULT_CONFIG)
    dt = cfg["CTRL_LOOP_PERIOD_MS"] / 1000.0
    # Deliberate gyro bias (deg/s), passed to both sensors and controller
    # (as if the board had calibrated it) -> checks the bias is removed correctly.
    bias = (1.5, -0.8, 0.4)
    n = [0]

    print("Unit checks")
    # Mixer: throttle 1500, roll command +100, limits 1100..2000.
    m = fm.mixer(cfg, 1500, 100, 0, 0, 1100, 2000)
    check(m[0] > m[2] and m[1] > m[3], f"+roll -> left motors (M1, M2) faster {m}")
    m = fm.mixer(cfg, 1500, 0, 100, 0, 1100, 2000)
    check(m[0] > m[1] and m[2] > m[3], f"+pitch -> front motors (M1, M3) faster {m}")
    # Throttle 1950 + roll 200 would push a motor past 2000 -> shift everything down.
    m = fm.mixer(cfg, 1950, 200, 0, 0, 1100, 2000)
    check(m[0] == 2000 and m[0] - m[2] == 400, f"saturation keeps differential {m}")
    p = fm.PID(2, 1, 0, 5, 10, 0)
    check(abs(p.update(1, 0, 0.1, True) - 2.1) < 1e-9, "PID P+I first step")

    print(f"Attitude control ({1 / dt:.0f} Hz loop, rigid body held in the air)")
    fc = fm.FlightController(cfg, bias)
    pl = fm.QuadPlant()
    pl.on_ground = False
    pl.fixed_position = True           # attitude-only test (gimbal rig)
    pl.thrust_us = [1500.0] * 4        # motors already spinning, no spool-up wait
    # One disarmed step to initialise the attitude estimate before arming.
    fc.step(*pl.sensors(bias), dt, 1000, 0, 0, 0)
    check(fc.arm(1000) == "Armed", "arm with throttle low")
    # Initial disturbance: roll 10 deg, pitch -8 deg, roll rate 1 rad/s.
    pl.roll, pl.pitch, pl.p = math.radians(10), math.radians(-8), 1.0
    er, ep, ey, pk = run(fc, pl, 3, 1500, 0, 0, 0, bias, dt, n)
    check(er < 1 and ep < 1, f"recover 10/-8 deg -> err {er:.2f}/{ep:.2f} deg")
    er, ep, ey, pk = run(fc, pl, 3, 1500, 20, -15, 0, bias, dt, n)
    check(er < 1.5 and ep < 1.5, f"track roll 20 / pitch -15 -> err {er:.2f}/{ep:.2f}, peak {pk:.1f}")
    er, ep, ey, pk = run(fc, pl, 3, 1500, 0, 0, 90, bias, dt, n)
    check(ey < 5 and er < 2, f"yaw rate 90 dps -> err {ey:.2f} dps")
    er, ep, ey, pk = run(fc, pl, 3, 1500, 0, 0, 0, bias, dt, n)
    check(er < 1 and ep < 1 and ey < 3, f"back to level -> {er:.2f} {ep:.2f} {ey:.2f}")

    print("Take-off and hover (full 6-DOF model)")
    fc = fm.FlightController(cfg, bias)
    pl = fm.QuadPlant()                # starts on the ground, motors stopped
    fc.step(*pl.sensors(bias), dt, 1000, 0, 0, 0)
    fc.arm(1000)
    hover = pl.hover_us()
    run(fc, pl, 2, hover + 60, 0, 0, 0, bias, dt, n)
    alt = -pl.pos[2]                   # NED: altitude = -z
    check(alt > 0.3, f"climbs with throttle {hover + 60:.0f} us -> altitude {alt:.2f} m")
    er, ep, _, _ = run(fc, pl, 3, hover, 0, 0, 0, bias, dt, n)
    drift = math.hypot(pl.pos[0], pl.pos[1])   # horizontal drift from the start point
    check(er < 1 and ep < 1 and drift < 1.0, f"stays level near hover: err {er:.2f}/{ep:.2f} deg, drift {drift:.2f} m")
    # pitch nose DOWN (-10 deg) must accelerate forward (+x north when yaw=0)
    pl2 = fm.QuadPlant(); fc2 = fm.FlightController(cfg, bias)
    fc2.step(*pl2.sensors(bias), dt, 1000, 0, 0, 0); fc2.arm(1000)
    run(fc2, pl2, 2, hover + 60, 0, 0, 0, bias, dt, n)
    x0 = pl2.pos[0]
    # A bit more than hover throttle so the tilted drone does not sink to the ground.
    run(fc2, pl2, 2, hover + 40, 0, -10, 0, bias, dt, n)
    check(pl2.pos[0] - x0 > 0.5, f"nose down 10 deg -> moves forward {pl2.pos[0] - x0:.2f} m")

    print("GPS position hold (USER_PosHold.c port, 5 Hz noisy GPS)")

    def gps_run(fc, pl, ph, secs, thr, heading_err_deg=0.0):
        """Fly with POSHOLD; GPS sampled at 5 Hz with +-0.3 m / 0.1 m/s noise.
        Returns (final distance to target, max |tilt setpoint| seen)."""
        steps = int(secs / dt)
        gps_every = max(1, int(0.2 / dt))
        gp = (0.0, 0.0, 0.0, 0.0)
        max_sp = 0.0
        for k in range(steps):
            n[0] += 1
            if k % gps_every == 0:
                nz = math.sin(n[0] * 1.7) * 0.3, math.cos(n[0] * 2.3) * 0.3
                gp = (pl.pos[0] + nz[0], pl.pos[1] + nz[1],
                      pl.vel[0] + nz[1] / 3, pl.vel[1] + nz[0] / 3)
            heading = math.degrees(pl.yaw) + heading_err_deg
            sr, sp = ph.update(gp[0], gp[1], gp[2], gp[3], heading, dt)
            max_sp = max(max_sp, abs(sr), abs(sp))
            g, a = pl.sensors(bias, noise=0.3, n=n[0])
            m = fc.step(g, a, dt, thr, sr, sp, 0)
            for _ in range(10):
                pl.step(m, dt / 10)
        dist = math.hypot(pl.pos[0] - ph.target_n, pl.pos[1] - ph.target_e)
        return dist, max_sp

    def gps_setup(yaw_deg):
        fc = fm.FlightController(cfg, bias)
        pl = fm.QuadPlant()
        pl.yaw = math.radians(yaw_deg)
        fc.step(*pl.sensors(bias), dt, 1000, 0, 0, 0)
        fc.arm(1000)
        run(fc, pl, 2, hover + 60, 0, 0, 0, bias, dt, n)     # take off
        ph = fm.PosHold(cfg)
        ph.start(pl.pos[0], pl.pos[1])
        return fc, pl, ph

    max_ang = cfg["POSHOLD_MAX_ANGLE_DEG"]
    for yaw_deg in (0, 90, -135):
        fc, pl, ph = gps_setup(yaw_deg)
        pl.pos[0] += 4.0                 # pushed 4 m North, 3 m West
        pl.pos[1] -= 3.0
        d, msp = gps_run(fc, pl, ph, 15, hover + 10)
        check(d < 0.6 and msp <= max_ang + 1e-6 and -pl.pos[2] > 0.2,
              f"nose {yaw_deg:4d} deg: 5 m push -> back within {d:.2f} m, max tilt sp {msp:.1f} deg")

    fc, pl, ph = gps_setup(30)
    pl.wind = [0.0, 3.0, 0.0]            # 3 m/s wind blowing towards East
    d, msp = gps_run(fc, pl, ph, 25, hover + 10)
    check(d < 0.7, f"3 m/s side wind, 25 s -> holds within {d:.2f} m (I term {ph.i_n:.1f}/{ph.i_e:.1f} deg)")

    # Negative control: heading wrong by 180 deg (e.g. compass mounted
    # backwards) must make the hold run AWAY -> proves the test can see it.
    fc, pl, ph = gps_setup(0)
    pl.pos[0] += 2.0
    d, _ = gps_run(fc, pl, ph, 8, hover + 10, heading_err_deg=180.0)
    check(d > 5.0, f"heading 180 deg wrong -> runs away ({d:.1f} m), test is meaningful")

    print("Negative control")
    # Deliberately flip the sign of the whole MIX_*_ROLL column (simulating a
    # USER_Config.h typo). The physics model does not use this table, so the
    # controller now pushes the WRONG way -> it must diverge. If it did not,
    # the simulation would be unable to detect sign errors and every test
    # above would be meaningless.
    cfg_bad = dict(cfg)
    for i in range(1, 5):
        cfg_bad[f"MIX_M{i}_ROLL"] = -cfg[f"MIX_M{i}_ROLL"]
    fc = fm.FlightController(cfg_bad, bias)
    pl = fm.QuadPlant(); pl.on_ground = False; pl.fixed_position = True; pl.thrust_us = [1500.0] * 4
    fc.step(*pl.sensors(bias), dt, 1000, 0, 0, 0); fc.arm(1000)
    pl.roll = math.radians(5)          # small push; a wrong sign makes it grow
    _, _, _, pk = run(fc, pl, 1.5, 1500, 0, 0, 0, bias, dt, n)
    # Divergence is shown by a large roll peak, or by the FAILSAFE tilt cut-off.
    check(pk > 45 or fc.state == "FAILSAFE", f"wrong roll sign diverges (peak {pk:.0f} deg, state {fc.state})")

    print("Telemetry parser")
    t = fm.parse_telemetry("$TLM,ARMED,-1.25,3.50,10.00,1100,1200,1300,1400,1350,2")
    check(t and t["state"] == "ARMED" and t["motors"][3] == 1400 and t["variant"] == 2, "parse $TLM")
    t = fm.parse_telemetry("> ACC[g] X=0.01, Y=-0.02, Z=0.99| ANGLE[deg] P=1.50, R=-2.25, Y=30.00")
    check(t and t["roll"] == -2.25 and t["pitch"] == 1.5, "parse legacy attitude line")

    print("\nALL PASSED" if not fails else f"\nFAILED ({fails})")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
