/*
  Lorawan.cpp - Network based functions using lorawan
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Lorawan.h"
#include "Arduino.h"
#include <TheThingsNetwork.h>

/**
 * 
 */
Lorawan::Lorawan(Stream &modemStream, Stream &debugStream, ttn_fp_t fp, uint8_t sf, uint8_t fsb)
  : TheThingsNetwork(modemStream, debugStream, fp, sf, fsb)
{
}

/*
 * 
 */
boolean Lorawan::join() 
{
  provision(appEui, appKey);
  
  for (; sf < 13; sf++) 
  {
    TheThingsNetwork::join(3, 10000); 
  }

  return false;
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
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}

/*
 * 
 */
ttn_response_t Lorawan::sendStillAlive(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 0;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, true, sf);
}

/*
 * 
 */
ttn_response_t Lorawan::sendGenericError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 2;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}

/*
 * 
 */
ttn_response_t Lorawan::sendMicrocontrollerError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 3;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}

/*
 * 
 */
ttn_response_t Lorawan::sendSensorError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 4;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}

/*
 * 
 */
ttn_response_t Lorawan::sendBatteryError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 5;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}

/*
 * 
 */
ttn_response_t Lorawan::sendStorageError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 6;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}

/*
 * 
 */
uint8_t Lorawan::getSpreadFactor()
{
  return sf;
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
  
  this->sf = spreadFactor;
  setSF(spreadFactor);
  saveState();
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


