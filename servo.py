#!/usr/bin/python

import pantilthat
import sys
import time

pan_angle = int(sys.argv[1])
tilt_angle = int(sys.argv[2])

if __name__ == "__main__":
    pantilthat.tilt(tilt_angle)
    pantilthat.pan(pan_angle)
    time.sleep(0.5)
