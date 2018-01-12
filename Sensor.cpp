/*
  Sensor.cpp - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Arduino.h"
#include "Sensor.h"
#include "Processor.h"


/**
 * 
 */
Sensor::Sensor(SDCard *sdCard, Processor *processor)
{
  // pinMode(pin, OUTPUT);
  analogPin = 0;
  rangeDifferenceThreshold = 50;
  this->processor = processor;
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


/*
 * 
 */
bool Sensor::sendMeasurement(int currentMeasurement)
{
  Serial.println("Measurement Sent:");
  Serial.println(currentMeasurement);
  return true;
}


/*
 * 
 */
void Sensor::attemptToSendMeasurement(int currentMeasurement)
{
  if(isCurrentWorthSending(currentMeasurement))
  {
    if(sendMeasurement(currentMeasurement))
      lastMeasurementSent = currentMeasurement;
      this->processor->printToSDLog(lastMeasurementSent);
  }
  //else ignore current measurement
  
}

/*
 * 
 */
void Sensor::startReadingProcess() {

  currentRiverLevel = getCurrentMeasurement();
  this->processor->printCurrentMeasurementToSD(getCurrentMeasurement());
  attemptToSendMeasurement(currentRiverLevel);
}

