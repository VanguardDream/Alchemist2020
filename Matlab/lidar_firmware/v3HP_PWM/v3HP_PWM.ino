/*------------------------------------------------------------------------------

  LIDARLite Arduino Library
  v3HP/v3HP_PWM

  This example shows how to read distance from a LIDAR-Lite connected over the
  PWM interface.

  Connections:
  LIDAR-Lite 5 Vdc (red) to Arduino 5v
  LIDAR-Lite Ground (black) to Arduino GND
  LIDAR-Lite Mode control (yellow) to Arduino digital input (pin 3)
  LIDAR-Lite Mode control (yellow) to 1 kOhm resistor lead 1
  1 kOhm resistor lead 2 to Arduino digital output (pin 2)

  Optional Connections -
      LIDAR-Lite I2C SCL (green) to Arduino SCL
      LIDAR-Lite I2C SDA (blue) to Arduino SDA

  (Capacitor recommended to mitigate inrush current when device is enabled)
  680uF capacitor (+) to Arduino 5v
  680uF capacitor (-) to Arduino GND

  See the Operation Manual for wiring diagrams and more information:
  http://static.garmin.com/pumac/LIDAR-Lite_v3HP_Instructions_EN.pdf
------------------------------------------------------------------------------*/

#include <stdint.h>
#include <Wire.h>
#include "LIDARLite_v3HP.h"

LIDARLite_v3HP myLidarLite;

#define MonitorPin    3
#define TriggerPin    2

uint32_t distance;
uint32_t txTime;
uint32_t startTime;
uint32_t endTime;
bool     newDistance = false;
bool     measuring   = false;

// -----------------------------------------
// Add in the #define below to illustrate
// optional use of I2C during PWM mode
// -----------------------------------------
//#define USE_I2C

void setup()
{
    Serial.begin(9600); // Start serial communications
    
    pinMode(MonitorPin, INPUT);
    pinMode(TriggerPin, OUTPUT);
    digitalWrite(TriggerPin, LOW); // Set trigger LOW for continuous read

    startTime = micros();
    endTime   = startTime;
    txTime = startTime;
}

void loop()
{
  
    if (digitalRead(MonitorPin))
    {
        if (measuring == false)
        {
            startTime   = micros();
            measuring   = true;
        }
    }
    else
    {
        if (measuring == true)
        {
            endTime     = micros();
            measuring   = false;
            newDistance = true;
        }
    }

    // If we get a new reading, print it
    if (newDistance == true)
    {
        distance = (endTime - startTime) / 10; // 10usec = 1 cm of distance
        
        //Serial.write((byte*)&distance,sizeof(distance)); // Print measured distance
        if((micros() - txTime) > 100000)
        {
         uint8_t clamp_dist = distance % 256;
         Serial.write(clamp_dist);
         txTime = micros();
        }
        
        newDistance = false;
    }
}
