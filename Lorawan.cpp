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
Lorawan::Lorawan(uint8_t spreadFactor) : ttn(Serial1, Serial, freqPlan, spreadFactor)
{
  setSpreadFactor(spreadFactor);
}

/*
 * 
 */
bool Lorawan::join() 
{
  return ttn.join(appEui, appKey);
}

/*
 * 
 */
ttn_response_t Lorawan::sendReading(int16_t reading, uint8_t powerLevel)
{
  byte data[6];
  data[0] = 1;
  data[1] = powerLevel;
  data[2] = highByte(reading);
  data[3] = lowByte(reading);
  
  return ttn.sendBytes(data, sizeof(data));
}

/*
 * 
 */
ttn_response_t Lorawan::sendStillAlive(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 1;
  data[1] = powerLevel;
  
  return ttn.sendBytes(data, sizeof(data));
}

/*
 * 
 */
ttn_response_t Lorawan::sendGenericError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 2;
  data[1] = powerLevel;
  
  return ttn.sendBytes(data, sizeof(data));
}

/*
 * 
 */
ttn_response_t Lorawan::sendMicrocontrollerError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 3;
  data[1] = powerLevel;
  
  return ttn.sendBytes(data, sizeof(data));
}

/*
 * 
 */
ttn_response_t Lorawan::sendSensorError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 4;
  data[1] = powerLevel;
  
  return ttn.sendBytes(data, sizeof(data));
}

/*
 * 
 */
ttn_response_t Lorawan::sendBatteryError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 5;
  data[1] = powerLevel;
  
  return ttn.sendBytes(data, sizeof(data));
}

/*
 * 
 */
ttn_response_t Lorawan::sendStorageError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 6;
  data[1] = powerLevel;
  
  return ttn.sendBytes(data, sizeof(data));
}

/*
 * 
 */
uint8_t Lorawan::getSpreadFactor()
{
  return spreadFactor;
}

/*
 * 
 */
void Lorawan::setSpreadFactor(uint8_t spreadfactor)
{
  if(spreadFactor < 7) {
    spreadFactor = 7;
  } else if (spreadFactor > 12) {
    spreadFactor = 12;
  }
  
  this->spreadFactor = spreadFactor;
  // Set SpreadFactor on TTN
}

/*
 * 
 */
char* Lorawan::getAppEui()
{
  return appEui;
}

/*
 * 
 */
void Lorawan::setAppEui(char *appEui)
{
  delete[] this->appEui;
  this->appEui = appEui;
}

/*
 * 
 */
char* Lorawan::getAppKey()
{
  return appKey;
}


/*
 * 
 */
void Lorawan::setAppKey(char *appKey)
{
  delete[] this->appKey;
  this->appKey = appKey;
}


