/*
  EngineeringMenu.h - Library for dealing with processor settings i.e. its interrupt/led pins.
  Created by Natalie Mclaren, December 18, 2017.
*/

#include <SPI.h>
#include <SD.h>
#include "Arduino.h"
#include "Processor.h"
#include "SDCard.h"
#include "Sensor.h"
#include "EngineeringMenu.h"


/*
 * 
 */
Processor::Processor(Sensor *sensor, SDCard *sdCard, EngineeringMenu *menu)
{
  this->sensor = sensor;
  this->sdCard = sdCard;
  this->menu = menu;
  
  state = LOW;
  delayPeriod = 5000;
  delayPeriodARMode = 1000;
  ARModeActivationThreshold = 20000; //Threshold (mm) to trigger Accelerated Readings mode
                                     //defaulted to a value that should never trigger (20 meters - out of range of sensor)
  ignoreThreshold = 0; //Threshold (mm) for which the sensor should ignore readings - don't send any info to server
}

Processor::adjustARModeDelay(int newDelayPeriod){ //adjust accelerated readings mode with new frequent delay period
  delayPeriodARMode = 1000;
}

Processor::adjustARModeThreshold(int newActivationThreshold){ //adjust accelerated readings mode with new activation threshold (mm)
  ARModeActivationThreshold = newActivaionThreshold;
}

Processor::activateOrDeactivateARMode() { // swaps delayPeriod and delayPeriodARMode variables to activate/deactivate Accelerated Readings mode
  int tempPeriod = 0;
  tempPeriod = delayPeriod;
  delayPeriod = delayPeriodARMode;
  delayPeriodARMode = tempPeriod;
}

Processor::adjustIgnoreThreshold(int newIgnoreThreshold){ 
  ignoreThreshold = newIgnoreThreshold;
}

/*
 * 
 */
void Processor::changeMeasurementPeriod(String minutes)
{
  // Update the delay period
  this->delayPeriod = minutes.toInt() * 60000;
}

void Processor::printToSDLog(int lastMeasurementSent)
{
  this->sdCard->printToLog(lastMeasurementSent);
}

void Processor:printCurrentMeasurementToSD(int currentMeasurement)
{
  this->sdCard->printCurrentMeasurement(currentMeasurement);
}



