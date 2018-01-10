/*
  Processor.h - Library for dealing with processor settings i.e. its interrupt/led pins.
  Created by Natalie Mclaren, December 18, 2017.
*/

#include <SPI.h>
#include <SD.h>
#ifndef Processor_h
#define Processor_h
#include "EngineeringMenu.h"
#include "SDCard.h"
#include "Sensor.h"

#include "Arduino.h"

/**
 * 
 */
class Processor
{
  
  public:
    Processor();
    const byte ledPin = 1;
    const byte interruptPin = 13;
    volatile byte state;
    int delayPeriod;

  private:

};

#endif
