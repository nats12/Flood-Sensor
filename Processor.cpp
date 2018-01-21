/*
  EngineeringMenu.h - Library for dealing with processor settings i.e. its interrupt/led pins.
  Created by Natalie Mclaren, December 18, 2017.
*/

#include "Processor.h"
#include <SPI.h>
#include <SD.h>
#include "Arduino.h"
#include "SDCard.h"
#include "Sensor.h"

/*
 * 
 */
Processor::Processor(Sensor *sensor, SDCard *sdCard, TheThingsNetwork *ttn, byte ledPin, byte interruptPin)
{
  this->sensor = sensor;
  this->sdCard = sdCard;
  this->ttn = ttn;
  
  this->ledPin = ledPin;
  this->interruptPin = interruptPin;
  
  this->state = LOW;
  this->delayPeriod = 5000;
}


// States

/*
 * 
 */
void Processor::init()
{
    ttn->join(appEui, appKey);
  
    String inStr;
    int initialDistanceToRiverTop;
    int currentDistanceToRiverTop;

    // Whilst inStr is null, do nothing, skip
    while ((inStr = Serial.readString()) == NULL){}
    initialRiverDepfth = inStr.toInt();
    //Serial.println(initialRiverDepth); 
    initialDistanceToRiverTop = analogRead(sensor->analogPin) * 5;
    sensor->distanceToRiverBed = initialRiverDepth + initialDistanceToRiverTop;
}


/*
 * 
 */
void Processor::readingProcess()
{
  sensor->startReadingProcess();
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



