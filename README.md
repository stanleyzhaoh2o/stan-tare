This is a project for tare calibration.
Refer to: https://github.com/realsenseai/librealsense/blob/master/doc/installation.md for the RealSense SDK installation.

Tare calibration first starts a depth stream, measures the distance at the center pixel, and then use the distance as the ground truth for tare calibration. It then runs the calibration while displaying progress. Afterwards it optionally saves the resulting calibration to the device flash memory.


