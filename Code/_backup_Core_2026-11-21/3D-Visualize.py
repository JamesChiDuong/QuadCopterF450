import serial
import re
import numpy as np
from vpython import *

# Serial port setup
ser = serial.Serial('COM3', 115200, timeout=1)

# Data line pattern
pattern = re.compile(
    r"X=(-?\d+\.\d+), Y=(-?\d+\.\d+), Z=(-?\d+\.\d+)\|.*P=(-?\d+\.\d+), R=(-?\d+\.\d+), Y=(-?\d+\.\d+)"
)

# VPython scene
scene.title = "3D Drone View (Drone Axes: X=Right, Y=Front, Z=Up)"
scene.forward = vector(0, -1, -0.5)  # Camera direction
scene.up = vector(0, 0, 1)
scene.background = color.white

# Drone body
body = box(size=vector(0.3, 0.3, 0.05), color=color.gray(0.5), opacity=0.8)

# Arms
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
    return vector(v[0], v[1], v[2])

def euler_to_matrix(pitch, roll, yaw):
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
    return Rz @ Rx @ Ry


# Live loop
while True:
    rate(60)
    line = ser.readline().decode(errors='ignore').strip()
    match = pattern.search(line)
    if match:
        ax, ay, az, pitch, roll, yaw = map(float, match.groups())

        R = euler_to_matrix(pitch, roll, yaw)

        # Axes according to your drone
        front = R @ np.array([0, 1, 0])   # Forward (Y)
        up    = R @ np.array([0, 0, 1])   # Up (Z)

        body.axis = np_to_v(front)
        body.up = np_to_v(up)

        arm_dirs = [
            R @ np.array([ 1,  1, 0]),
            R @ np.array([-1,  1, 0]),
            R @ np.array([-1, -1, 0]),
            R @ np.array([ 1, -1, 0]),
        ]
        for i in range(4):
            arms[i].pos = body.pos
            arms[i].axis = np_to_v(arm_dirs[i]) * arm_len
            props[i].pos = arms[i].pos + arms[i].axis
