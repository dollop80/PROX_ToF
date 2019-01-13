#include "mavlink.h"        // Mavlink interface
#include "common/mavlink_msg_distance_sensor.h"

#include <Wire.h>
#include <VL53L1X.h>
const int MIN = 10;
const int MAX = 3000;

/*
Valid values are (even numbers only):
Pre: 12 to 18 (initialized to 14 by default)
Final: 8 to 14 (initialized to 10 by default)
*/
const int PreRng = 18;  
const int PostRng = 14;

const int Scale = 10;
#define bRate 115200


int XSHUT_pins[9] = {8, 9, 10, 11, 12, 13, 6, 7, 5};
//int orient[9] = {0, 1, 2, 3, 4, 5, 6, 7, 24};
int orient[9] = {0, 7, 6, 5, 4, 3, 2, 1, 24};


//ADDRESS_DEFAULT 0x52
uint8_t addr[9] = {90, 83, 84, 85, 86, 87, 88, 89, 91};

float SensorSmooth[9];
float SensorFilter[9];

VL53L1X sensor[9];

void setup()
{ /*WARNING*/
  //Shutdown pins of VL53L0X ACTIVE-LOW-ONLY NO TOLERANT TO 5V will fry them
  for (int i=0; i<sizeof(XSHUT_pins);i++)
  {
    pinMode(XSHUT_pins[i], OUTPUT);
    }

  Serial.begin(115200);
  
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  
  for(int i = 7; i >= 0; i--)
  {

    pinMode(XSHUT_pins[i], INPUT);
    delay(10);
    
    sensor[i].setTimeout(500);
    if(!sensor[i].init())
      Serial.println("Failed to detect and initialize sensor!");  
    sensor[i].setAddress(addr[i]);

  }

  for(int i = 7; i >= 0; i--)
  {
    // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
    // You can change these settings to adjust the performance of the sensor, but
    // the minimum timing budget is 20 ms for short distance mode and 33 ms for
    // medium and long distance modes. See the VL53L1X datasheet for more
    // information on range and timing limits.
    sensor[i].setDistanceMode(VL53L1X::Long);
    sensor[i].setMeasurementTimingBudget(35000);
  
    // Start continuous readings at a rate of one measurement every 50 ms (the
    // inter-measurement period). This period should be at least as long as the
    // timing budget.
    sensor[i].startContinuous(33);
  }
}

void loop()
{

/*
for(int i = 6; i>=2; i--)
{
   SensorSmooth[i] = sensor[i].readRangeContinuousMillimeters();
   SensorFilter[i] = constrain(SensorSmooth[i], MIN , MAX);
   Serial.print(SensorFilter[i]); 
   Serial.print(",");
}

  Serial.println();
  delay (0.1);
*/
  for(int i = 7; i >= 0; i--){
    command_distance(&sensor[i], orient[i]);
  }
}



void command_heartbeat() {

  //< ID 1 for this system
  int sysid = 100;                   
  //< The component sending the message.
  int compid = MAV_COMP_ID_PATHPLANNER;    
  
  // Define the system type, in this case ground control station
  uint8_t system_type =MAV_TYPE_GCS;
  uint8_t autopilot_type = MAV_AUTOPILOT_INVALID;
  
  uint8_t system_mode = 0; 
  uint32_t custom_mode = 0;                
  uint8_t system_state = 0;
  
  // Initialize the required buffers
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  
  // Pack the message
  mavlink_msg_heartbeat_pack(sysid,compid, &msg, system_type, autopilot_type, system_mode, custom_mode, system_state);
  
  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
  
  // Send the message 
  //delay(1);
  Serial.write(buf, len);
}


void command_distance(VL53L1X *sensor, int orient) {

// READ THE DISTANCE SENSOR
  float SensorSmooth = sensor->readRangeContinuousMillimeters();
  SensorSmooth = constrain(SensorSmooth, MIN , MAX);
  float dist = SensorSmooth / Scale;

  //MAVLINK DISTANCE MESSAGE
  int sysid = 1;                   
  //< The component sending the message.
  int compid = 158;

  uint32_t time_boot_ms = 0; /*< Time since system boot*/
  uint16_t min_distance = 1; /*< Minimum distance the sensor can measure in centimeters*/
  uint16_t max_distance = 300; /*< Maximum distance the sensor can measure in centimeters*/
  uint16_t current_distance = dist; /*< Current distance reading*/
  uint8_t type = 0; /*< Type from MAV_DISTANCE_SENSOR enum.*/
  uint8_t id = orient; /*< Onboard ID of the sensor*/
  uint8_t orientation = orient; /*(0=forward, each increment is 45degrees more in clockwise direction), 24 (upwards) or 25 (downwards)*/
// Consumed within ArduPilot by the proximity class

  uint8_t covariance = 0; /*< Measurement covariance in centimeters, 0 for unknown / invalid readings*/


  // Initialize the required buffers
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  // Pack the message
  mavlink_msg_distance_sensor_pack(sysid,compid,&msg,time_boot_ms,min_distance,max_distance,current_distance,type,id,orientation,covariance);

  // Copy the message to the send buffer
  uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

  Serial.write(buf, len);
}
