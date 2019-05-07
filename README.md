# drillGunBoard

Code for FRC control board. Manual control FRC motors and air compressor.

We use 12v drills to power 12v FRC motors, (CIMs, BAGs, etc) by removing drill spindle and connecting to motor leads. However, it can not supply large amounts of power. It can not drive Neo motors due to their brushless nature. This code is for a board with Talon SRX, or other motor controllers, and possibly an air compressor output to compress air while disabled.
