#!/usr/bin/python

import time
import picamera

camera = picamera.PiCamera()
camera.start_preview()
time.sleep(10)
camera.stop_preview()
camera.close()
