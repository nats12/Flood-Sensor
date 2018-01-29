/*
  EngineeringMenu.h - Library for dealing with processor settings i.e. its interrupt/led pins.
  Created by Natalie Mclaren, December 18, 2017.
*/
#define VBATPIN A7

#include "Processor.h"
#include <SPI.h>
#include <SD.h>
#include "Arduino.h"
#include "SDCard.h"
#include "Sensor.h"

/*
 * 
 */
Processor::Processor(Sensor *sensor, SDCard *sdCard, byte ledPin, byte interruptPin) //TheThingsNetwork *ttn, byte ledPin, byte interruptPin)
{
  this->sensor = sensor;
  this->sdCard = sdCard;
//  this->ttn = ttn;
  
  this->ledPin = ledPin;
  this->interruptPin = interruptPin;
  
  this->state = LOW;
  this->delayPeriod = 5000;
  this->delayPeriodARMode = 1000;
  this->ARModeActivationThreshold = 20000; //Threshold (mm) to trigger Accelerated Readings mode
                                     //defaulted to a value that should never trigger (20 meters - out of range of sensor)
  this->ignoreThreshold = 0; //Threshold (mm) for which the sensor should ignore readings - don't send any info to server
}

// State Loop functions

/*
 * 
 */
void Processor::init()
{
    //ttn->join(appEui, appKey);
  
    String inStr;
    int initialDistanceToRiverTop;
    int currentDistanceToRiverTop;

    // Whilst inStr is null, do nothing, skip
    while ((inStr = Serial.readString()) == NULL){}
    initialRiverDepth = inStr.toInt();
    //Serial.println(initialRiverDepth); 
    initialDistanceToRiverTop = analogRead(sensor->analogPin) * 5;
    sensor->distanceToRiverBed = initialRiverDepth + initialDistanceToRiverTop;
}

/*
 * 
 */
float Processor::getBatteryVoltage()
 {
  float measuredvbat = analogRead(VBATPIN);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  return measuredvbat;
 }


/*
 * 
 */
void Processor::readingProcess()
{
  int currentRiverLevel = sensor->getCurrentMeasurement();
  
  if(sensor->isCurrentWorthSending(currentRiverLevel))
  {
    bool sent = false;
    if(sent) {
      sensor->lastMeasurementSent = currentRiverLevel;
      //storage.printToLog(lastMeasurementSent);
    }
  }
}

// AR Mode
void Processor::adjustARModeDelay(int newDelayPeriod){ //adjust accelerated readings mode with new frequent delay period
  delayPeriodARMode = 1000;
}

void Processor::adjustARModeThreshold(int newActivationThreshold){ //adjust accelerated readings mode with new activation threshold (mm)
  ARModeActivationThreshold = newActivationThreshold;
}

void Processor::activateOrDeactivateARMode() { // swaps delayPeriod and delayPeriodARMode variables to activate/deactivate Accelerated Readings mode
  int tempPeriod = 0;
  tempPeriod = delayPeriod;
  delayPeriod = delayPeriodARMode;
  delayPeriodARMode = tempPeriod;
}

void Processor::adjustIgnoreThreshold(int newIgnoreThreshold){ 
  ignoreThreshold = newIgnoreThreshold;
}


// Helpers
/*
 * 
 */
void Processor::writeStatus()
{
  digitalWrite(this->ledPin, this->state);
}

/*
 * 
 */
void Processor::delayWithPeriod()
{
  Serial.println("Current measurement period is..");
  Serial.println(this->delayPeriod);
  delay(this->delayPeriod);
}

// Setters

/*
 * 
 */
void Processor::setSpreadingFactor(int spreadFactor)
{
  this->spreadFactor = spreadFactor;
}

/*
 * 
 */
void Processor::setAppEui(char *appEui)
{
  this->appEui = appEui;
}

/*
 * 
 */
void Processor::setAppKey(char *appKey)
{
  this->appKey = appKey;
}

/*
 * 
 */
void Sensor::changeMeasurementPeriod(int minutes)
{
  this->measurementPeriod = minutes * 60000;
}

/*
 * 
 */
void Processor::printToSDLog(int lastMeasurementSent)
{
  this->sdCard->printToLog(lastMeasurementSent);
}

/*
 * 
 */
void Processor::printCurrentMeasurementToSD(int currentMeasurement)
{
  this->sdCard->printCurrentMeasurement(currentMeasurement);
}


