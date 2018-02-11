/*
  Sensor.cpp - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Sensor.h"
#include "Arduino.h"

/**
 * 
 */
Sensor::Sensor(uint8_t analogPin)
{
  analogPin = analogPin;
  rangeDifferenceThreshold = 50;
}

/*
 * 
 */
int16_t Sensor::getCurrentMeasurement()
{
  // Read a raw value
  int16_t rawVal = analogRead(analogPin);
  // As per datasheet (to get mm)   
  int16_t currentDistanceToRiverTop = rawVal * 5;  
  // Output value  

  return distanceToRiverBed - currentDistanceToRiverTop;
}



/*
 * 
 */
boolean Sensor::isCurrentWorthSending(int16_t currentMeasurement)
{ 
  return (abs(currentMeasurement - lastMeasurementSent)) >= rangeDifferenceThreshold;
}

