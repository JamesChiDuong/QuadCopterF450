"""Shared model of the QuadCopter flight controller + an F450 physics model.

The controller code is a line-by-line port of the firmware
(USER_PID.c, USER_Attitude.c, USER_Mixer.c, the cascade in USER_System.c).
All constants are read from USER_Config.h, so the simulation always uses the
same gains / limits / mixer signs as the firmware.

Frames: body FRD (x forward, y right, z down), world NED.
roll > 0 right side down, pitch > 0 nose up, yaw > 0 nose right.

The plant parameters below are ESTIMATES for an F450 with A2212 930KV and
10" props on 3S; they are good enough to check signs and stability, not to
predict exact behaviour.

===========================================================================
OVERVIEW
===========================================================================
Purpose
    Shared library for the PC-side Python tools. This file is NOT run on its
    own; it is imported by:
      - sim_flight_controller.py : automatic regression test (no board needed).
      - drone_viewer.py          : 3D viewer (--sim mode uses this model; live
                                   mode uses parse_telemetry()).

The file has 4 parts:
  1. load_config()      - reads the constants from the firmware's USER_Config.h.
  2. The controller     - PID, Attitude, mixer(), FlightController: a
                          LINE-BY-LINE port of the C firmware:
                            USER_PID.c      -> class PID
                            USER_Attitude.c -> class Attitude (complementary filter)
                            USER_Mixer.c    -> mixer() (Mixer_QuadX)
                            USER_System.c   -> FlightController.step() (cascade:
                                               angle P -> rate PID -> mixer)
                          If you change an algorithm in the .c files you MUST
                          make the same change here, otherwise the simulation
                          no longer represents the firmware.
  3. QuadPlant          - 6-DOF rigid-body physics model of the F450 frame.
  4. parse_telemetry()  - splits a UART line ($TLM from DEBUG=6, or the legacy
                          DEBUG=1 line) into a dict.

Why read the constants from USER_Config.h?
    PID gains, limits and mixer signs are never copied by hand into Python.
    On every run load_config() parses the firmware header directly, so the
    simulation always uses exactly the gains the firmware will use. Change a
    gain in USER_Config.h, re-run sim_flight_controller.py, and you know
    immediately whether the new gain is sane.

Coordinate frames (every sign in the project depends on these)
    - Body FRD: x towards the nose (Forward), y to the Right, z Down.
    - World NED: x = North, y = East, z = Down. Because z points down,
      altitude = -z.
    - roll > 0  : RIGHT side goes down.
      pitch > 0 : nose goes UP.
      yaw > 0   : nose turns RIGHT (clockwise seen from above).

F450 physics model (QuadPlant)
    - Thrust of each motor is linear in pulse width above 1000 us:
      T = KT * (us - 1000). Motors have first-order lag (MOTOR_TAU).
    - Roll/pitch torques come from the motor GEOMETRY: tau = r x F, with r the
      motor position and F = (0, 0, -T) (thrust points up = -z in FRD). Yaw
      torque is the propeller reaction torque (direction from SPIN).
    - KEY POINT: the torques are computed INDEPENDENTLY of the mixer sign
      table (MIX_Mx_ROLL/PITCH/YAW). So if the mixer table in USER_Config.h
      has a wrong sign, the simulation does not silently "agree" with it: the
      drone flips / diverges and the test catches the error. (If torques were
      derived from the mixer table itself, a wrong sign would still look
      correct.)
    - Ground contact: while resting on the ground and thrust does not exceed
      weight, the drone stays still and level. Touching the ground (z > 0)
      stops it.
    - fixed_position = True: a "gimbal test rig" - rotation only, no
      translation, as if the drone were mounted on a ball joint. Used to test
      the attitude loop in isolation.
    - All parameters are ESTIMATES (A2212 930KV, 10" props, 3S): good enough
      to check signs and stability, not to predict real flight precisely.

Telemetry parser
    - "$TLM,<state>,<roll>,<pitch>,<yaw>,<m1>,<m2>,<m3>,<m4>,<thr>[,<variant>]"
      sent by the firmware after @QC+DEBUG=6.
    - Legacy line "... X=.., Y=.., Z=..| ... P=.., R=.., Y=.." (DEBUG=1), as
      used by 3D-Visualize.py.
"""
import math
import os
import re

# Default path to USER_Config.h, relative to this file's folder (host/):
# host/../Code/1. Control BLDC/Core/Inc/USER_Config.h. Works no matter which
# directory the scripts are started from.
DEFAULT_CONFIG = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                              "..", "Code", "1. Control BLDC", "Core", "Inc",
                              "USER_Config.h")


def load_config(path=DEFAULT_CONFIG):
    """Return {NAME: float} for every numeric #define in USER_Config.h.

    The regex accepts the forms the firmware uses:
      #define A   4U            (U suffix)
      #define B   0.85f         (f suffix)
      #define C   (-1.0f)       (parentheses, sign)
      ... optionally followed by a /* ... */ or // ... comment.
    #defines whose value is not a plain number (macros, expressions) are
    skipped.
    """
    cfg = {}
    with open(path, encoding="utf-8") as f:
        for line in f:
            m = re.match(r"\s*#define\s+(\w+)\s+\(?\s*([-+]?\d+(?:\.\d+)?)[fU]?\s*\)?\s*(?:/[*/].*)?$", line)
            if m:
                cfg[m.group(1)] = float(m.group(2))
    return cfg


def clamp(v, lim):
    """Limit v to [-lim, +lim] (symmetric clamp, as in the firmware)."""
    return max(-lim, min(lim, v))


class PID:
    """Port of USER_PID.c

    Single-axis PID:
      - P = kp * error.
      - I = integral of ki * error * dt, clamped to [-i_limit, +i_limit]
            (anti-windup). Only integrates when integrate=True (the angle loop
            passes False because it is P-only).
      - D = derivative of the MEASUREMENT (not of the error), then a
            first-order low-pass at d_lpf_hz. Avoids "derivative kick" on
            setpoint steps and reduces gyro noise.
      - The output is clamped to [-out_limit, +out_limit].
    """

    def __init__(self, kp, ki, kd, i_limit, out_limit, d_lpf_hz):
        self.kp, self.ki, self.kd = kp, ki, kd
        self.i_limit, self.out_limit, self.d_lpf_hz = i_limit, out_limit, d_lpf_hz
        self.reset()

    def reset(self):
        """Clear the state (integrator, previous measurement, D filter)."""
        self.integ = 0.0
        self.prev = 0.0
        self.dfilt = 0.0
        self.first = True     # first update: no "prev" yet to differentiate against
        self.output = 0.0

    def update(self, sp, meas, dt, integrate):
        """One PID step. sp = setpoint, meas = measurement, dt in seconds."""
        if dt <= 0:
            return self.output
        err = sp - meas
        p = self.kp * err
        if integrate and self.ki > 0:
            self.integ = clamp(self.integ + self.ki * err * dt, self.i_limit)
        if self.first:
            # First call: use the current measurement as "prev" so D = 0,
            # instead of a huge D spike from prev = 0.
            self.prev, self.dfilt, self.first = meas, 0.0, False
        # D on measurement: -(d meas / dt). Minus sign because err = sp - meas.
        d_raw = -(meas - self.prev) / dt
        self.prev = meas
        if self.d_lpf_hz > 0:
            # First-order RC low-pass: rc = 1/(2*pi*fc), alpha = dt/(rc+dt).
            rc = 1.0 / (2 * math.pi * self.d_lpf_hz)
            self.dfilt += dt / (rc + dt) * (d_raw - self.dfilt)
        else:
            self.dfilt = d_raw
        self.output = clamp(p + self.integ + self.kd * self.dfilt, self.out_limit)
        return self.output


class Attitude:
    """Port of USER_Attitude.c (complementary filter, angles in degrees).

      - Integrates the gyro through the Euler kinematic equations (correct
        when tilted, not just adding p/q/r) -> fast and smooth but drifts.
      - Pulls roll/pitch slowly towards the accelerometer angles (direction of
        gravity) with time constant IMU_COMP_FILTER_TAU_S -> removes drift.
      - Only trusts the accelerometer when its magnitude is close to 1 g
        (between IMU_ACC_TRUST_MIN_G and IMU_ACC_TRUST_MAX_G); ignores it
        during strong vibration / acceleration.
      - Yaw is gyro-only (no magnetometer), so yaw drifts slowly over time.
    Units: angles in deg, gyro in deg/s, accel in g, all in body FRD.
    """

    def __init__(self, cfg, gyro_bias=(0.0, 0.0, 0.0)):
        self.cfg = cfg
        self.bias = list(gyro_bias)   # gyro offset (the firmware measures it while CALIBRATING)
        self.initialized = False
        self.roll = self.pitch = self.yaw = 0.0
        self.roll_rate = self.pitch_rate = self.yaw_rate = 0.0

    def update(self, gyro_dps, acc_g, dt):
        """Update the estimate with one gyro + accel sample taken dt seconds later."""
        p = gyro_dps[0] - self.bias[0]
        q = gyro_dps[1] - self.bias[1]
        r = gyro_dps[2] - self.bias[2]
        # The rate PIDs use the bias-corrected body rates directly.
        self.roll_rate, self.pitch_rate, self.yaw_rate = p, q, r
        ax, ay, az = acc_g
        # Accel angles in FRD: when level, acc = (0, 0, -1) g (specific force
        # points up = -z). Hence roll = atan2(-ay, -az),
        # pitch = atan2(ax, sqrt(ay^2 + az^2)).
        acc_roll = math.degrees(math.atan2(-ay, -az))
        acc_pitch = math.degrees(math.atan2(ax, math.hypot(ay, az)))
        norm = math.sqrt(ax * ax + ay * ay + az * az)
        if not self.initialized:
            # First sample: initialise directly from the accelerometer (yaw = 0).
            self.roll, self.pitch, self.yaw = acc_roll, acc_pitch, 0.0
            self.initialized = True
            return
        rr = math.radians(self.roll)
        # Clamp pitch to +-1.4 rad (~80 deg) so tan(pitch) and 1/cos(pitch)
        # stay finite (Euler-angle singularity / gimbal lock at 90 deg).
        pr = max(-1.4, min(1.4, math.radians(self.pitch)))
        sr, cr, tp = math.sin(rr), math.cos(rr), math.tan(pr)
        # ZYX Euler kinematics: body rates (p, q, r) -> Euler angle rates.
        self.roll += (p + (q * sr + r * cr) * tp) * dt
        self.pitch += (q * cr - r * sr) * dt
        self.yaw += ((q * sr + r * cr) / math.cos(pr)) * dt
        # Wrap roll and yaw to [-180, 180).
        self.roll = (self.roll + 180) % 360 - 180
        self.yaw = (self.yaw + 180) % 360 - 180
        if self.cfg["IMU_ACC_TRUST_MIN_G"] < norm < self.cfg["IMU_ACC_TRUST_MAX_G"]:
            # Complementary filter weight: a close to 1 -> trust the gyro,
            # move only a little towards the accel angle each step.
            tau = self.cfg["IMU_COMP_FILTER_TAU_S"]
            a = tau / (tau + dt)
            self.roll = a * self.roll + (1 - a) * acc_roll
            self.pitch = a * self.pitch + (1 - a) * acc_pitch


def mixer(cfg, thr, roll, pitch, yaw, lo, hi):
    """Port of Mixer_QuadX. Returns [M1..M4] in us.

        M_i = thr + MIX_Mi_ROLL*roll + MIX_Mi_PITCH*pitch + MIX_Mi_YAW*yaw
    The MIX_* signs come from USER_Config.h. Motor order: M1 FL (front left),
    M2 BL (back left), M3 FR (front right), M4 BR (back right).

    Saturation: instead of clamping each motor separately (which destroys the
    differences between motors = loses attitude authority), all 4 motors are
    SHIFTED together down if the largest exceeds `hi`, then shifted up if the
    smallest is below `lo`. The differential (torque) is kept and only total
    thrust is sacrificed. The low limit wins (it is applied last) so motors
    never stop in the air; the final clamp only bites when the spread between
    motors is larger than (hi - lo).
    """
    mix = [[cfg[f"MIX_M{i}_ROLL"], cfg[f"MIX_M{i}_PITCH"], cfg[f"MIX_M{i}_YAW"]]
           for i in range(1, 5)]
    m = [thr + mix[i][0] * roll + mix[i][1] * pitch + mix[i][2] * yaw for i in range(4)]
    mx, mn = max(m), min(m)
    if mx > hi:
        m = [v - (mx - hi) for v in m]
        mn -= mx - hi
    if mn < lo:
        m = [v + (lo - mn) for v in m]
    # +0.5 then int() = round to the nearest us (like the cast in C).
    return [int(min(hi, max(lo, v)) + 0.5) for v in m]


class FlightController:
    """Cascade of USER_System.c (angle P -> rate PID -> mixer) + arming.

    Mirrors the control task in USER_System.c, run every CTRL_LOOP_PERIOD_MS:
      1. Attitude.update()     : estimate roll/pitch/yaw.
      2. Outer (angle) loop, P : angle error [deg] -> desired rate [deg/s],
                                 clamped to SP_MAX_RATE_DPS.
      3. Inner (rate) loop     : PID -> roll/pitch/yaw commands [us].
         Yaw has no angle loop: the pilot commands yaw RATE directly.
      4. mixer()               : 4 motor commands [us].
    Plus the safety logic: arming conditions, FAILSAFE on excessive tilt, and
    low throttle -> motors at idle with PIDs reset.
    PID names: RA/PA = Roll/Pitch Angle, RR/PR/YR = Roll/Pitch/Yaw Rate.
    """

    def __init__(self, cfg, gyro_bias=(0.0, 0.0, 0.0)):
        c = self.cfg = cfg
        self.att = Attitude(cfg, gyro_bias)
        self.pid = {
            # Angle loop: P only (ki = kd = 0); output is the desired rate.
            "RA": PID(c["PID_ANGLE_ROLL_KP"], 0, 0, 0, c["SP_MAX_RATE_DPS"], 0),
            "PA": PID(c["PID_ANGLE_PITCH_KP"], 0, 0, 0, c["SP_MAX_RATE_DPS"], 0),
            # Rate loop: full PID; output is a differential motor command [us].
            "RR": PID(c["PID_RATE_ROLL_KP"], c["PID_RATE_ROLL_KI"], c["PID_RATE_ROLL_KD"],
                      c["PID_RATE_I_LIMIT"], c["PID_RATE_OUT_LIMIT"], c["PID_D_LPF_HZ"]),
            "PR": PID(c["PID_RATE_PITCH_KP"], c["PID_RATE_PITCH_KI"], c["PID_RATE_PITCH_KD"],
                      c["PID_RATE_I_LIMIT"], c["PID_RATE_OUT_LIMIT"], c["PID_D_LPF_HZ"]),
            # Yaw has its own (smaller) output limit so yaw cannot eat the
            # throttle headroom that roll/pitch need.
            "YR": PID(c["PID_RATE_YAW_KP"], c["PID_RATE_YAW_KI"], c["PID_RATE_YAW_KD"],
                      c["PID_RATE_I_LIMIT"], c["PID_YAW_OUT_LIMIT"], c["PID_D_LPF_HZ"]),
        }
        self.armed = False
        self.state = "DISARMED"
        self.motors = [int(c["MOTOR_STOP_US"])] * 4

    def reset_pids(self):
        """Reset every PID (clears integrators). Called on arm/disarm/low throttle."""
        for p in self.pid.values():
            p.reset()

    def arm(self, throttle_us):
        """Try to ARM. Returns a result string like the firmware's message.

        Conditions (same as firmware): not already armed, frame nearly level
        (|roll|, |pitch| <= SAFE_ARM_MAX_TILT_DEG), throttle low.
        On arming the PIDs are reset and yaw is zeroed (current heading = 0).
        """
        c = self.cfg
        if self.armed:
            return "Already armed"
        if abs(self.att.roll) > c["SAFE_ARM_MAX_TILT_DEG"] or abs(self.att.pitch) > c["SAFE_ARM_MAX_TILT_DEG"]:
            return "Frame is tilted"
        if throttle_us > c["RC_THROTTLE_LOW_US"]:
            return "Throttle not low"
        self.reset_pids()
        self.att.yaw = 0.0
        self.armed, self.state = True, "ARMED"
        return "Armed"

    def disarm(self, state="DISARMED"):
        """Stop the motors (MOTOR_STOP_US). state="FAILSAFE" for a safety cut."""
        self.armed, self.state = False, state
        self.reset_pids()
        self.motors = [int(self.cfg["MOTOR_STOP_US"])] * 4

    def step(self, gyro_dps, acc_g, dt, thr_us, sp_roll, sp_pitch, sp_yawrate):
        """One control period. Returns motor commands [us].

        sp_roll / sp_pitch are desired angles [deg], sp_yawrate is the desired
        yaw rate [deg/s], thr_us is the throttle [us].
        """
        c = self.cfg
        # The attitude is always updated, even when disarmed (arming checks need it).
        self.att.update(gyro_dps, acc_g, dt)
        if not self.armed:
            self.motors = [int(c["MOTOR_STOP_US"])] * 4
            return self.motors
        # FAILSAFE: tilt beyond SAFE_CRASH_TILT_DEG = crashed/flipped, cut motors.
        if abs(self.att.roll) > c["SAFE_CRASH_TILT_DEG"] or abs(self.att.pitch) > c["SAFE_CRASH_TILT_DEG"]:
            self.disarm("FAILSAFE")
            return self.motors
        idle, mmax = c["MOTOR_IDLE_US"], c["MOTOR_MAX_US"]
        if thr_us <= c["RC_THROTTLE_LOW_US"]:
            # Low throttle (sitting on the ground): motors at idle, PIDs reset
            # so the integrators cannot wind up while the frame cannot rotate.
            self.reset_pids()
            self.motors = mixer(c, idle, 0, 0, 0, idle, mmax)
            return self.motors
        # Cascade: angle -> desired rate -> differential motor command.
        rsp = self.pid["RA"].update(sp_roll, self.att.roll, dt, False)
        psp = self.pid["PA"].update(sp_pitch, self.att.pitch, dt, False)
        ur = self.pid["RR"].update(rsp, self.att.roll_rate, dt, True)
        up = self.pid["PR"].update(psp, self.att.pitch_rate, dt, True)
        uy = self.pid["YR"].update(sp_yawrate, self.att.yaw_rate, dt, True)
        self.motors = mixer(c, thr_us, ur, up, uy, idle, mmax)
        return self.motors


# --------------------------------------------------------------------------
# Physics (F450 estimate)
# The "plant" that the controller above has to stabilise.
# --------------------------------------------------------------------------
class QuadPlant:
    """6-DOF physics model of the F450 frame (rigid body + 4 motors).

    State: Euler angles roll/pitch/yaw [rad], body rates p/q/r [rad/s],
    position/velocity in NED [m, m/s], lagged thrust of each motor [us].
    Torques are computed from motor positions (MX, MY) and spin directions
    (SPIN) - the MIX_* table is NOT used - so a wrong mixer sign makes the
    simulated drone diverge.
    """
    ARM = 0.225          # m, centre to motor
    KT = 0.0078          # N per us above 1000 us (per motor)
    KY = 0.00025         # N.m reaction torque per us
    IXX, IYY, IZZ = 0.0075, 0.0075, 0.013   # kg.m^2, moments of inertia about body x/y/z
    MASS = 1.2           # kg
    MOTOR_TAU = 0.04     # s   (motor + ESC first-order time constant)
    DRAG = 0.35          # linear drag, 1/s   (translation only)
    # motor geometry, FRD. spin +1 = CCW seen from above (M1 FL, M2 BL, M3 FR, M4 BR)
    # MX = +1: motor is in front; MY = +1: motor is on the right (units of ARM*cos45).
    # SPIN = +1: prop turns CCW seen from above -> its reaction torque on the
    # body is clockwise seen from above = positive yaw (nose right).
    # NOTE: this M1..M4 order must match the real motor wiring and the MIX_*
    # table in USER_Config.h.
    MX = (1, -1, 1, -1)     # M1 front, M2 back, M3 front, M4 back
    MY = (-1, -1, 1, 1)     # M1 left,  M2 left, M3 right, M4 right
    SPIN = (-1, 1, 1, -1)   # this frame: M1 CW, M2 CCW, M3 CCW, M4 CW ("props out")

    def __init__(self):
        self.roll = self.pitch = self.yaw = 0.0     # rad
        self.p = self.q = self.r = 0.0              # rad/s body
        self.pos = [0.0, 0.0, 0.0]                  # NED, m (z down: altitude = -z)
        self.vel = [0.0, 0.0, 0.0]
        self.thrust_us = [1000.0] * 4
        self.on_ground = True
        self.fixed_position = False   # True: rotation only (gimbal test rig)
        # Steady wind: the air moves at this NED speed [m/s]; drag pulls the
        # frame towards it (used by the GPS position-hold test).
        self.wind = [0.0, 0.0, 0.0]

    def rotation(self):
        """Body -> NED rotation matrix (ZYX).

        Standard aerospace yaw-pitch-roll order. Column 3 (R[i][2]) is the
        body z axis (down) expressed in NED, i.e. the opposite of the thrust
        direction.
        """
        cr, sr = math.cos(self.roll), math.sin(self.roll)
        cp, sp = math.cos(self.pitch), math.sin(self.pitch)
        cy, sy = math.cos(self.yaw), math.sin(self.yaw)
        return [[cy * cp, cy * sp * sr - sy * cr, cy * sp * cr + sy * sr],
                [sy * cp, sy * sp * sr + cy * cr, sy * sp * cr - cy * sr],
                [-sp, cp * sr, cp * cr]]

    def step(self, cmd_us, dt):
        """Advance the physics by dt seconds with motor commands cmd_us [us] (explicit Euler)."""
        # Distance of each motor from the body x / y axis = ARM * cos(45 deg) (X frame).
        k = 0.7071 * self.ARM
        tx = ty = tz = total = 0.0
        for i in range(4):
            # First-order lag of the motor towards its command (rotor + ESC inertia).
            self.thrust_us[i] += (cmd_us[i] - self.thrust_us[i]) * dt / self.MOTOR_TAU
            t = self.KT * max(0.0, self.thrust_us[i] - 1000.0)
            total += t
            # r = (MX*k, MY*k, 0), F = (0, 0, -t)  =>  r x F = (-MY*k*t, MX*k*t, 0).
            # Example: a right-hand motor (MY=+1) pushing harder gives tx < 0
            # -> negative roll (right side rises). Pure physics, no mixer.
            tx += -(self.MY[i] * k) * t          # tau = r x F, F = (0,0,-T)
            ty += (self.MX[i] * k) * t
            tz += self.SPIN[i] * self.KY * max(0.0, self.thrust_us[i] - 1000.0)

        if self.fixed_position:
            # Gimbal rig: ignore gravity / translation, rotate only.
            self.on_ground = False
            self._rotate(tx, ty, tz, dt)
            return

        # translational dynamics (NED)
        # Thrust acts along body -z, rotated into NED; add gravity (+9.81
        # because z points down) and subtract linear drag.
        R = self.rotation()
        acc = [R[i][2] * (-total) / self.MASS for i in range(3)]
        acc[2] += 9.81
        for i in range(3):
            acc[i] -= self.DRAG * (self.vel[i] - self.wind[i])

        if self.on_ground and acc[2] >= 0:
            # resting on the ground: no motion, attitude held level
            # (thrust does not exceed weight yet, the ground holds the frame)
            self.vel = [0.0, 0.0, 0.0]
            self.p = self.q = self.r = 0.0
            self.roll = self.pitch = 0.0
            return
        self.on_ground = False

        for i in range(3):
            self.vel[i] += acc[i] * dt
            self.pos[i] += self.vel[i] * dt
        if self.pos[2] > 0:                    # touched the ground (z > 0 = below ground level)
            self.pos[2] = 0.0
            self.vel = [0.0, 0.0, 0.0]
            self.on_ground = True

        self._rotate(tx, ty, tz, dt)

    def _rotate(self, tx, ty, tz, dt):
        """Rotational dynamics: torque -> angular acceleration -> rates -> Euler angles.

        Simplified: the gyroscopic term (omega x I*omega) is ignored, which is
        small at moderate rates.
        """
        self.p += tx / self.IXX * dt
        self.q += ty / self.IYY * dt
        self.r += tz / self.IZZ * dt
        # Same Euler kinematics as Attitude.update().
        sr, cr = math.sin(self.roll), math.cos(self.roll)
        tp, cp = math.tan(self.pitch), math.cos(self.pitch)
        self.roll += (self.p + (self.q * sr + self.r * cr) * tp) * dt
        self.pitch += (self.q * cr - self.r * sr) * dt
        self.yaw += ((self.q * sr + self.r * cr) / cp) * dt

    def sensors(self, gyro_bias=(0.0, 0.0, 0.0), noise=0.0, n=0):
        """Ideal MPU6050 in body axes: gyro deg/s, accel (specific force) g.

        Gyro = true rate + bias + deterministic "noise" (a sine of the step
        counter n, so tests are repeatable). Note: the accel returned here is
        only the gravity direction for the current roll/pitch (idealised); it
        does not include the body's linear acceleration.
        """
        nz = noise * math.sin(n * 1.7)
        gyro = (math.degrees(self.p) + gyro_bias[0] + nz,
                math.degrees(self.q) + gyro_bias[1] - nz,
                math.degrees(self.r) + gyro_bias[2] + nz)
        acc = (math.sin(self.pitch),
               -math.sin(self.roll) * math.cos(self.pitch),
               -math.cos(self.roll) * math.cos(self.pitch))
        return gyro, acc

    def hover_us(self):
        """Throttle [us] at which the 4 motors exactly carry MASS (hover)."""
        return 1000.0 + self.MASS * 9.81 / 4 / self.KT


# --------------------------------------------------------------------------
# GPS position hold (port of USER_PosHold.c - keep identical)
# --------------------------------------------------------------------------
class PosHold:
    """Position P -> ground speed PI -> tilt, rotated by the heading.

    Positions / speeds in metres, local North / East. heading_deg: 0 = nose
    North, 90 = nose East. update() returns (roll_sp_deg, pitch_sp_deg) in
    the firmware FRD convention (pitch < 0 = nose down = fly forward).
    """

    def __init__(self, cfg):
        self.g = dict(pos_p=cfg["POSHOLD_POS_P"], max_speed=cfg["POSHOLD_MAX_SPEED_MS"],
                      vel_p=cfg["POSHOLD_VEL_P"], vel_i=cfg["POSHOLD_VEL_I"],
                      vel_i_limit=cfg["POSHOLD_VEL_I_LIMIT"],
                      max_angle=cfg["POSHOLD_MAX_ANGLE_DEG"])
        self.target_n = self.target_e = 0.0
        self.i_n = self.i_e = 0.0
        self.active = False

    def start(self, pos_n, pos_e):
        self.target_n, self.target_e = pos_n, pos_e
        self.i_n = self.i_e = 0.0
        self.active = True

    def update(self, pos_n, pos_e, vel_n, vel_e, heading_deg, dt):
        g = self.g
        if not self.active:
            return 0.0, 0.0
        sp_vn = (self.target_n - pos_n) * g["pos_p"]
        sp_ve = (self.target_e - pos_e) * g["pos_p"]
        ln = math.hypot(sp_vn, sp_ve)
        if ln > g["max_speed"]:
            sp_vn *= g["max_speed"] / ln
            sp_ve *= g["max_speed"] / ln
        err_vn, err_ve = sp_vn - vel_n, sp_ve - vel_e
        self.i_n = clamp(self.i_n + err_vn * g["vel_i"] * dt, g["vel_i_limit"])
        self.i_e = clamp(self.i_e + err_ve * g["vel_i"] * dt, g["vel_i_limit"])
        tilt_n = err_vn * g["vel_p"] + self.i_n
        tilt_e = err_ve * g["vel_p"] + self.i_e
        c, s = math.cos(math.radians(heading_deg)), math.sin(math.radians(heading_deg))
        fwd = c * tilt_n + s * tilt_e
        right = -s * tilt_n + c * tilt_e
        ln = math.hypot(fwd, right)
        if ln > g["max_angle"]:
            fwd *= g["max_angle"] / ln
            right *= g["max_angle"] / ln
        return right, -fwd


# --------------------------------------------------------------------------
# Telemetry parsing (firmware -> host)
# --------------------------------------------------------------------------
# $TLM (DEBUG=6): state, roll, pitch, yaw, M1..M4, throttle, [optional variant].
_TLM = re.compile(r"\$TLM,(\w+),(-?[\d.]+),(-?[\d.]+),(-?[\d.]+),(\d+),(\d+),(\d+),(\d+),(-?[\d.]+)(?:,(\d+))?")
# Legacy (DEBUG=1): accel X/Y/Z, then angles P(itch)/R(oll)/Y(aw).
_LEGACY = re.compile(r"X=(-?\d+\.\d+), Y=(-?\d+\.\d+), Z=(-?\d+\.\d+)\|.*P=(-?\d+\.\d+), R=(-?\d+\.\d+), Y=(-?\d+\.\d+)")


def parse_telemetry(line):
    """Parse one UART line. Returns a dict or None.

    $TLM,<state>,<roll>,<pitch>,<yaw>,<m1>,<m2>,<m3>,<m4>,<thr>[,<variant>]  (DEBUG=6)
    legacy '... X=.., Y=.., Z=..| ... P=.., R=.., Y=..'                       (DEBUG=1)

    Returns {state, roll, pitch, yaw, motors, throttle, variant}; fields the
    line does not carry are None (the legacy line only has angles). Uses
    search(), not match(), so leading junk (e.g. '\\r') is tolerated.
    Non-telemetry lines (logs, command replies) -> None.
    """
    m = _TLM.search(line)
    if m:
        g = m.groups()
        return {"state": g[0], "roll": float(g[1]), "pitch": float(g[2]), "yaw": float(g[3]),
                "motors": [int(g[4]), int(g[5]), int(g[6]), int(g[7])],
                "throttle": float(g[8]), "variant": int(g[9]) if g[9] else None}
    m = _LEGACY.search(line)
    if m:
        # Drop the 3 accel values; the angles appear in the order P, R, Y.
        _, _, _, pitch, roll, yaw = map(float, m.groups())
        return {"state": None, "roll": roll, "pitch": pitch, "yaw": yaw,
                "motors": None, "throttle": None, "variant": None}
    return None
