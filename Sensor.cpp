/*
  Sensor.cpp - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Arduino.h"
#include "Sensor.h"
#include "SDCard.h"
#include "Processor.h"


/**
 * 
 */
Sensor::Sensor(SDCard *sdCard, Processor *processor)
{
  // pinMode(pin, OUTPUT);
  analogPin = 0;
  rangeDifferenceThreshold = 50;
  this->sdCard = sdCard;
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
      this->sdCard->printToLog(lastMeasurementSent);
  }
  //else ignore current measurement
  
}

/*
 * 
 */
void Sensor::startReadingProcess() {

  currentRiverLevel = getCurrentMeasurement();
  this->sdCard->printCurrentMeasurement(getCurrentMeasurement());
  attemptToSendMeasurement(currentRiverLevel);
}



/*
 * 
 */
void Sensor::changeMeasurementPeriod(String minutes)
{
  // Update the delay period
  this->processor->delayPeriod = minutes.toInt();
}

