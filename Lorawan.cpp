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
Lorawan::Lorawan(uint8_t spreadFactor) : ttn(Serial1, Serial, freqPlan, 8)
{
  setSpreadFactor(spreadFactor);
}

/*
 * 
 */
boolean Lorawan::join() 
{
  ttn.provision(appEui, appKey);
  return ttn.join();
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
  data[0] = 0;
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
  return "70B3D57EF00000C3";
}

/*
 * 
 */
void Lorawan::setAppEui(char *appEui)
{
  delete[] this->appEui;
  this->appEui = "70B3D57EF00000C3";
}

/*
 * 
 */
char* Lorawan::getAppKey()
{
  return "ttn-account-v2._GgABDAVOs2zzGe7Jb8xqE8z1jNPsJNgDXheXo3OpwY";
}


/*
 * 
 */
void Lorawan::setAppKey(char *appKey)
{
  delete[] this->appKey;
  this->appKey = "ttn-account-v2._GgABDAVOs2zzGe7Jb8xqE8z1jNPsJNgDXheXo3OpwY";
}


