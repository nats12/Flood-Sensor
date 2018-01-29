/*
  Sensor.cpp - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Lorawan.h"
#include "Arduino.h"
#include <TheThingsNetwork.h>

/**
 * 
 */
Lorawan::Lorawan(int spreadFactor)
{
  setSpreadFactor(spreadFactor);
}

int Lorawan::join() 
{
  
}

/*
 * 
 */
int Lorawan::getSpreadFactor()
{
  return spreadFactor;
}

/*
 * 
 */
void Lorawan::setSpreadFactor(int spreadfactor)
{
  if(spreadFactor < 7) {
    spreadFactor = 7;
  } else if (spreadFactor > 12) {
    spreadFactor = 12;
  }
  
  this->spreadFactor = spreadFactor;
}

