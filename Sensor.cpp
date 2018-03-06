/*
  Sensor.cpp - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Sensor.h"
#include "Arduino.h"

/**
 * Sensor class constructor.
 * Sets default range difference threshold (anything below this threshold in mm, should not be sent to api).
 * @param {uint8_t} {analogPin} Serial input pin for ultrasonic sensor output.
 * @return N/A
 */
Sensor::Sensor(uint8_t analogPin)
{
  analogPin = analogPin;
}

/*
 * Use ultrasonic sensor to calculate the current river depth level in mm.
 * @param N/A
 * @return {int16_t} current river depth level measurement (mm).
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
 * Check if the current measurement is different enough from the last one sent (worth sending?).
 * @param {int16_t} {currentMeasurement} current river depth measurement taken to compare to last measurement sent.
 * @return {bool} is worth sending or not (true or false) - Difference between the last measurement sent and current is more than or equal to the rangeDifferenceThreshold set.
 */
bool Sensor::isCurrentWorthSending(int16_t currentMeasurement)
{ 
  return (abs(currentMeasurement - lastMeasurementSent)) >= rangeDifferenceThreshold;
}

