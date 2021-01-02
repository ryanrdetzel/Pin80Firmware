# Pin80Firmware
Teensy firmware for Pin80 Controller

Serial commands

To update RGB pixels issue a start, update the pixels, then issue an end.
PXSTART
P4 PX0 001600
P4 PX1,3,5 160000
P4 PX0-6 160000
PXEND

Multiple commands can be send sperated by newline
PXSTART\nP4 PX0 001600\nPXEND\n

Non pixel ports can be triggered with
P0 ON
P0 OFF