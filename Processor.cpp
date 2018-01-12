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
Processor::Processor()
{
  state = LOW;
  delayPeriod = 5000;

}

/*
 * 
 */
void Processor::changeMeasurementPeriod(String minutes)
{
  // Update the delay period
  this->delayPeriod = minutes.toInt();
}

void Processor::printToSDLog(int lastMeasurementSent)
{
  this->sdCard->printToLog(lastMeasurementSent);
}

void Processor:printCurrentMeasurementToSD(int currentMeasurement)
{
  this->sdCard->printCurrentMeasurement(currentMeasurement);
}



