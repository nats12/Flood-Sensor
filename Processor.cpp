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

}



