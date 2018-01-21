/*
  Sensor.cpp - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Sensor.h"
#include "Arduino.h"

/**
 * 
 */
Sensor::Sensor(int analogPin)
{
  analogPin = analogPin;
  rangeDifferenceThreshold = 50;
}

/*
 * 
 */
int Sensor::getCurrentMeasurement()
{
  // Read a raw value
  int rawVal = analogRead(analogPin);
  // As per datasheet (to get mm)   
  int currentDistanceToRiverTop = rawVal * 5;  
  // Output value  

  return distanceToRiverBed - currentDistanceToRiverTop;
}



/*
 * 
 */
bool Sensor::isCurrentWorthSending(int currentMeasurement)
{
  return (abs(currentMeasurement - lastMeasurementSent)) >= rangeDifferenceThreshold;
}

