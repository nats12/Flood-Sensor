/*
  EngineeringMenu.h - Library for dealing with processor settings i.e. its interrupt/led pins.
  Created by Natalie Mclaren, December 18, 2017.
*/

#include <SPI.h>
#include <SD.h>
#include "Arduino.h"
#include "Processor.h"


/*
 * 
 */
Processor::Processor()
{
  state = LOW;
  delayPeriod = 5000;

}



