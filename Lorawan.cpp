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
Lorawan::Lorawan(Stream &modemStream, Stream &debugStream, ttn_fp_t fp, uint8_t sf, uint8_t fsb)
{
  this->debugStream = &modemStream;
  this->modemStream = &debugStream;
  this->modemStream->setTimeout(10000);
  this->fp = fp; // Frequency Plan
  this->sf = sf; // Spread Factor
  this->fsb = fsb;
}

/*
 * 
 */
boolean Lorawan::join() 
{
  provision(appEui, appKey);
  int8_t retries = -1;
  uint32_t retryDelay = 10000;

  int8_t attempts = 0;
  configureChannels(fsb);
  
  for (uint8_t curSf = sf; curSf < 13; curSf++) 
  {
    setSF(sf);
      
    while (retries == -1 || attempts <= retries)
    {
      attempts++;
      if (!sendJoinSet(MAC_JOIN_MODE_OTAA))
      {
        debugPrintMessage(ERR_MESSAGE, ERR_JOIN_FAILED);
        delay(retryDelay);
        continue;
      }
      readLine(buffer, sizeof(buffer));
      if (pgmstrcmp(buffer, CMP_ACCEPTED) != 0)
      {
        debugPrintMessage(ERR_MESSAGE, ERR_JOIN_NOT_ACCEPTED, buffer);
        debugPrintMessage(ERR_MESSAGE, ERR_CHECK_CONFIGURATION);
        delay(retryDelay);
        continue;
      }
      readResponse(MAC_TABLE, MAC_CH_TABLE, MAC_CHANNEL_STATUS, buffer, sizeof(buffer));
      debugPrintMessage(SUCCESS_MESSAGE, SCS_JOIN_ACCEPTED, buffer);
      readResponse(MAC_TABLE, MAC_GET_SET_TABLE, MAC_DEVADDR, buffer, sizeof(buffer));
      debugPrintIndex(SHOW_DEVADDR, buffer);
      saveState();
      return true;
    }
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
  data[0] = 1;
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
  
  this->spreadFactor = spreadFactor;
  setSF(sf);
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


