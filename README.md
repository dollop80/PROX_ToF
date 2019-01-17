# Copter Obstacle Avoidance for Ardupilot - PROX_ToF
The obstacle avoidance module is based on ToF (Time of Flight) distance sensors VL53L1. The module is rather small and lightweight so it can fit even 140mm frame

## What do you need:
- PROX_ToF PCB and parts
- Ardupilot compatible flight controller (Pixhawk, Omnibus, Pixracer, etc)
- Optical flow sensor (FlowDeck or Cheerson CX-OF)
- Any AVR ISP programmer to flash bootloader
- Your favorite quad, tri, hexa, octo :)

## Preparing Hardware:
- Assemble your PROX_ToF module and flash a bootloader using AVR ISP and Arduino IDE (See Preparing Software)
- Print a case
- Prepare all the nessesary connectors and wires
- Put your optical flow sensor on the bottom of the case
- Connect the module to flight controller
[Connection diagram](http://ixbt.photo/photo/259904/60047adsphykfW1/1266301w.jpg)]

## Preparing Software:
- Flash a bootloader using AVR ISP and Arduino IDE if you haven't done it yet
- Compile and Upload the VL53L1_MULTI_MAVLINK.ino sketch. If you have any errors regarding libraries, install the libraries to your Arduino IDE from this repository
- Follow the instruction how to [build Ardupilot firmware](http://ardupilot.org/dev/docs/building-the-code.html);
- After cloning the repository and succesfull building the ArduCopter firmware, change your board's **hwdef.dat** file (in my case it is located \libraries\AP_HAL_ChibiOS\hwdef\omnibusf4pro\hwdef.dat bacause I used Omnibus F4 RPO FC):
1. Find the CS pins definitions (line #67) and add a line
```
PD2 FLOW_CS CS
```
2. Find the SPI Device table and comment the OSD SPI (SPI3 DEVID4). Instead the OSD add the optical flow device:
```
SPIDEV pixartflow SPI3 DEVID4 FLOW_CS MODE3 2MHZ 2MHZ
env DEFINES HAL_HAVE_PIXARTFLOW_SPI
```
3. Comment out the OSD defines (line #135)
```
#define OSD_ENABLED ENABLED
#define HAL_OSD_TYPE_DEFAULT 1
```
- Configure and build the copter firmware. Flash your FC
- Using MissionPlanner adjust the following parameters:
```
EK2_ALT_SOURCE = 1
EK2_GPS_TYPE = 3
FLOW_ENABLE = 1
RNGFND_TYPE = 16
RNGFND_MAX_CM = 140
```
## Checking that everything works
- Connect to your FC using MissionPlanner and press Ctrl+F button
- Push the Proximity button
- Check that you see the Radar and it reacts to distance change
[](http://ardupilot.org/dev/_images/code-overview-object-avoidance4.png)
- Arm and takeoff in Altitude Hold mode, you'll notice that your drone will react on closing up objects trying to avoid any possible collision. Try Loiter.

## My thanks
Thanks to patrickpoirier51 work https://github.com/patrickpoirier51/POC it was a good start for MAVLINK DISTANCE MESSAGE generation
Thanks to [Vitaly Puzrin](http://forum.rcdesign.ru/member.php?u=349) for general thoughts and instructions of writing README 
