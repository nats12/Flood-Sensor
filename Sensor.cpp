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


/*
 * 
 */
bool Sensor::sendMeasurement(int currentMeasurement)
{
  Serial.println("Measurement Sent:");
  Serial.println(currentMeasurement);
//  byte data[1];
//  data[0] = 2;
//  
//  ttn->sendBytes(data, sizeof(data));
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
      //storage.printToLog(lastMeasurementSent);
  }
  //else ignore current measurement
  
}

/*
 * 
 */
void Sensor::startReadingProcess() 
{
  currentRiverLevel = getCurrentMeasurement();
  //storage.printCurrentMeasurement();
  attemptToSendMeasurement(currentRiverLevel);
}


/*
 * 
 */
void Sensor::changeMeasurementPeriod(int minutes)
{
  this->measurementPeriod = minutes;
}

