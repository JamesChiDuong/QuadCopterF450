# ===========================================================================
# OVERVIEW
# ===========================================================================
# Purpose
#   The OLD (legacy) 3D attitude viewer, kept because it still works with
#   firmware debug mode DEBUG=1. The newer drone_viewer.py does much more
#   ($TLM telemetry, motor colours, keyboard control, simulation); prefer it.
#
# How to run
#   python 3D-Visualize.py --port COM3 [--baud 115200]
#   The script sends "@QC+DEBUG=1" itself. The firmware then streams lines like
#     "> ACC[g] X=0.01, Y=-0.02, Z=0.99| ANGLE[deg] P=1.50, R=-2.25, Y=30.00"
#   and the regex below extracts accel X/Y/Z and pitch / roll / yaw.
#   Stop with Ctrl+C (nothing is sent to the board on exit; the stream keeps
#   running until you send @QC+DEBUG=0).
#
# Coordinate frames
#   The firmware uses body FRD (x forward, y right, z down):
#   roll > 0 right side down, pitch > 0 nose up, yaw > 0 nose right.
#   This viewer instead draws in its own frame: X = right, Y = forward,
#   Z = up (see scene.title).
#   - Roll is a rotation about Y (forward) and pitch about X (right); with
#     this frame the firmware's positive roll/pitch already rotate the right
#     way, so they are used as is.
#   - Yaw is NEGATED: in FRD, z points DOWN, so positive yaw is clockwise
#     seen from above. Here Z points UP, and a positive (right-hand) rotation
#     about +Z is counter-clockwise seen from above = nose LEFT. Without the
#     minus sign the drone on screen would turn the opposite way.
# ===========================================================================
import argparse
import serial
import re
import numpy as np
from vpython import *

# Usage: python 3D-Visualize.py --port COM3
# On the drone send "@QC+DEBUG=1" to start the attitude stream.
parser = argparse.ArgumentParser(description="QuadCopter attitude 3D viewer")
parser.add_argument("--port", default="COM3", help="serial port (default COM3)")
parser.add_argument("--baud", type=int, default=115200)
args = parser.parse_args()

# Serial port setup
ser = serial.Serial(args.port, args.baud, timeout=1)
ser.write(b"@QC+DEBUG=1\r\n")   # ask the firmware to stream attitude

# Data line pattern
# Groups: accel X, Y, Z [g], then P (pitch), R (roll), Y (yaw) [deg].
# Same regex as _LEGACY in fc_model.py.
pattern = re.compile(
    r"X=(-?\d+\.\d+), Y=(-?\d+\.\d+), Z=(-?\d+\.\d+)\|.*P=(-?\d+\.\d+), R=(-?\d+\.\d+), Y=(-?\d+\.\d+)"
)

# VPython scene
scene.title = "3D Drone View (Drone Axes: X=Right, Y=Front, Z=Up)"
scene.forward = vector(0, -1, -0.5)  # Camera direction   (from behind, looking forward and down)
scene.up = vector(0, 0, 1)
scene.background = color.white

# Drone body
body = box(size=vector(0.3, 0.3, 0.05), color=color.gray(0.5), opacity=0.8)

# Arms
# Front arms (+Y) red, rear arms blue, so the nose is easy to see.
arm_len = 0.25
arm_rad = 0.015
arms = [
    cylinder(pos=vector(0, 0, 0), axis=vector( arm_len,  arm_len, 0), radius=arm_rad, color=color.red),
    cylinder(pos=vector(0, 0, 0), axis=vector(-arm_len,  arm_len, 0), radius=arm_rad, color=color.red),
    cylinder(pos=vector(0, 0, 0), axis=vector(-arm_len, -arm_len, 0), radius=arm_rad, color=color.blue),
    cylinder(pos=vector(0, 0, 0), axis=vector( arm_len, -arm_len, 0), radius=arm_rad, color=color.blue),
]

# Propellers
prop_radius = 0.03
props = [sphere(radius=prop_radius, color=color.black) for _ in range(4)]

# Convert to VPython vector
def np_to_v(v):
    """numpy array (3,) -> vpython vector."""
    return vector(v[0], v[1], v[2])

def euler_to_matrix(pitch, roll, yaw):
    """Rotation matrix in the viewer frame (X right, Y forward, Z up) from degrees.

    pitch rotates about X, roll about Y, yaw about Z. The caller must already
    have negated yaw (see OVERVIEW).
    """
    # Convert degrees to radians
    pitch = np.radians(pitch)  # rotation around X (right)
    roll  = np.radians(roll)   # rotation around Y (forward)
    yaw   = np.radians(yaw)    # rotation around Z (up)

    Rx = np.array([
        [1, 0, 0],
        [0, np.cos(pitch), -np.sin(pitch)],
        [0, np.sin(pitch),  np.cos(pitch)]
    ])
    Ry = np.array([
        [np.cos(roll), 0, np.sin(roll)],
        [0, 1, 0],
        [-np.sin(roll), 0, np.cos(roll)]
    ])
    Rz = np.array([
        [np.cos(yaw), -np.sin(yaw), 0],
        [np.sin(yaw),  np.cos(yaw), 0],
        [0, 0, 1]
    ])

    # Correct rotation order: yaw (Z) → pitch (X) → roll (Y)
    # (Equivalent to the firmware's ZYX order yaw -> pitch -> roll, but with
    #  the viewer's axes: pitch is about X, roll about Y.)
    return Rz @ Rx @ Ry


# Live loop
while True:
    rate(60)          # at most 60 iterations/s (also lets vpython redraw)
    # readline() waits at most timeout=1 s; errors='ignore' drops corrupt bytes.
    line = ser.readline().decode(errors='ignore').strip()
    match = pattern.search(line)
    if match:
        # ax, ay, az (accel) are parsed but not used for drawing.
        ax, ay, az, pitch, roll, yaw = map(float, match.groups())
        # Firmware uses FRD (yaw > 0 = nose right); this viewer's Z axis is up,
        # where a positive rotation turns the nose left -> invert yaw.
        yaw = -yaw

        R = euler_to_matrix(pitch, roll, yaw)

        # Axes according to your drone
        front = R @ np.array([0, 1, 0])   # Forward (Y)
        up    = R @ np.array([0, 0, 1])   # Up (Z)

        # vpython orients a box by axis (length direction) + up.
        body.axis = np_to_v(front)
        body.up = np_to_v(up)

        # Unit directions of the 4 arms in the body frame (rotated by R).
        # Not normalised: length is sqrt(2), hence each arm is drawn
        # arm_len * sqrt(2) long - purely cosmetic.
        arm_dirs = [
            R @ np.array([ 1,  1, 0]),
            R @ np.array([-1,  1, 0]),
            R @ np.array([-1, -1, 0]),
            R @ np.array([ 1, -1, 0]),
        ]
        for i in range(4):
            arms[i].pos = body.pos
            arms[i].axis = np_to_v(arm_dirs[i]) * arm_len
            props[i].pos = arms[i].pos + arms[i].axis   # propeller at the tip of the arm
