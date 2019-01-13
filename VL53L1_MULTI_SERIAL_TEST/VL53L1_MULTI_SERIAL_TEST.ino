#include <Wire.h>
#include <VL53L1X.h>
const int MIN = 10;
const int MAX = 3000;

int XSHUT_pins[8] = {8, 9, 10, 11, 12, 13, 6, 7};

//ADDRESS_DEFAULT 0x52

uint8_t addr[8] = {90, 83, 84, 85, 86, 87, 88, 89};

float SensorSmooth[8];
float SensorFilter[8];

VL53L1X sensor[8];

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
  
  for(int i = 6; i>=2; i--)
  {

    pinMode(XSHUT_pins[i], INPUT);
    delay(10);
    
    sensor[i].setTimeout(500);
    if(!sensor[i].init())
      Serial.println("Failed to detect and initialize sensor!");  
    sensor[i].setAddress(addr[i]);

  }

  for(int i = 6; i>=2; i--)
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
for(int i = 6; i>=2; i--)
{
   SensorSmooth[i] = sensor[i].readRangeContinuousMillimeters();
   SensorFilter[i] = constrain(SensorSmooth[i], MIN , MAX);
   Serial.print(SensorFilter[i]); 
   Serial.print(",");
}

  Serial.println();
  delay (0.1);
}
