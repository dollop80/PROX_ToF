# PROX_ToF
Proximity module for Ardupilot based on ToF sensors
The module was inspired by patrickpoirier51 work https://github.com/patrickpoirier51/POC.

This module is smaller, based on newer sensors VL53L1 and contains in one box an OpticalFlow sensor - https://www.bitcraze.io/flow-deck/
It is possible to run the module with 3...9 sensors

The repositoty includes:
- two Arduino scetches: one for proximity visualization via Arduino-Serial-Plotter, and another to generate MAVLINK DISTANCE MESSAGE
- PCB project of sensors holder with ATMEGA368 and place to put magnetometer module (because I connected the PROX_ToF to omnibus_v3_pro that doesn't contain mag sensor)
- 3D models of the box
