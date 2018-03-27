/**
  Lorawan.cpp - Network based functions using lorawan
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Lorawan.h"
#include "Arduino.h"
#include <TheThingsNetwork.h>

/**
 * Constructor, including all other objects to be accessed.
 * Lorawan functions joining/sending data to the network.
 * @param {Stream} {&modenStream} reference to Stream object.
 * @param {Stream} {&debugStream} reference to Stream object.
 * @param {ttn_fp_t} {fp} 
 * @param {uint8_t} {sf} Lorawan spread factor
 * @param {uint8_t} {fsb} pointer to Lorawan object
 * @return {Void} N/A
 */
Lorawan::Lorawan(Stream &modemStream, Stream &debugStream, ttn_fp_t fp, uint8_t sf, uint8_t fsb)
  : TheThingsNetwork(modemStream, debugStream, fp, sf, fsb)
{
}

/**
 * Join the TTN.
 * @param N/A
 * @return {boolean} True if it joined TTN, false if it didn't
 */
bool Lorawan::join() 
{
  provision();
  
  for (; sf < 13; sf++) 
  {
    if(TheThingsNetwork::join(3, 10000)) {
      return true; 
    }
  }

  return false;
}


/**
 * Sets the information needed to activate the device.
 * @param {char} {appEui} The application EUI 
 * @param {char} {appKey} The application key 
 * @return {boolean} True if successful, false otherwise
 */
bool Lorawan::provision()
{
  return TheThingsNetwork::provision(appEui, appKey);
}




/**
 * Sends a reading.
 * @param {int16_t} {reading} A reading from the sensor
 * @param {uint8_t} {powerLevel} The processor's battery power level
 * @return {ttn_response_t} Returns a success or error code and logs the related error message i.e. 'successful transmisson'
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



/**
 * Sends a 'Still Here' code.
 * @param {uint8_t} {powerLevel} The processor's battery power level
 * @return {ttn_response_t} Returns a success or error code and logs the related error message i.e. 'successful transmisson'
 */
ttn_response_t Lorawan::sendStillAlive(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 0;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, true, sf);
}



/**
 * Sends a Generic Error code.
 * @param {uint8_t} {powerLevel} The processor's battery power level
 * @return {ttn_response_t} Returns a success or error code and logs the related error message i.e. 'successful transmisson'
 */
ttn_response_t Lorawan::sendGenericError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 2;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}


/**
 * Sends a Microcontroller Error code.
 * @param {uint8_t} {powerLevel} The processor's battery power level
 * @return {ttn_response_t} Returns a success or error code and logs the related error message i.e. 'successful transmisson'
 */
ttn_response_t Lorawan::sendMicrocontrollerError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 3;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}

/**
 * Sends a Sensor Error code.
 * @param {uint8_t} {powerLevel} The processor's battery power level
 * @return {ttn_response_t} Returns a success or error code and logs the related error message i.e. 'successful transmisson'
 */
ttn_response_t Lorawan::sendSensorError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 4;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}

/**
 * Sends a Battery Error code.
 * @param {uint8_t} {powerLevel} The processor's battery power level
 * @return {ttn_response_t} Returns a success or error code and logs the related error message i.e. 'successful transmisson'
 */
ttn_response_t Lorawan::sendBatteryError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 5;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}

/**
 * Sends a Storage Error code.
 * @param {uint8_t} {powerLevel} The processor's battery power level
 * @return {ttn_response_t} Returns a success or error code and logs the related error message i.e. 'successful transmisson'
 */
ttn_response_t Lorawan::sendStorageError(uint8_t powerLevel)
{
  byte data[2];
  data[0] = 6;
  data[1] = powerLevel;
  
  return sendBytes(data, sizeof(data), 1, false, sf);
}

/**
 * Getter to get the Lorawan spread factor used.
 * @param {N/A} 
 * @return {uint8_t} {SF} The spread factor.
 */
uint8_t Lorawan::getSpreadFactor()
{
  return sf;
}

/**
 * Setter to set the Lorawan spread factor used.
 * @param {uint8_t} {spreadFactor} The Lorawan spreadfactor 
 * @return {Void} 
 */
void Lorawan::setSpreadFactor(uint8_t spreadFactor)
{
  if(spreadFactor < 7) {
    spreadFactor = 7;
  } else if (spreadFactor > 12) {
    spreadFactor = 12;
  }
  
  this->sf = spreadFactor;
  setSF(spreadFactor);
}



/**
 * Getter to get the application's Eui.
 * @param {N/A}
 * @return {char} {appEui} The application Eui used
 */
char* Lorawan::getCharAppEui()
{
  return appEui;
}



/**
 * Setter to set the applications Eui.
 * @param {char} {appEui} Pointer to the application's Eui.
 * @return {Void}
 */
void Lorawan::setCharAppEui(char *appEui)
{
  delete[] this->appEui;
  this->appEui = appEui;
}

/**
 * Getter to get the application's key.
 * @param {N/A}
 * @return {char} {appKey} The application key used
 */
char* Lorawan::getAppKey()
{
  return appKey;
}


/**
 * Setter to set the applications key.
 * @param {char} {appKey} Pointer to the application's key.
 * @return {Void}
 */
void Lorawan::setAppKey(char *appKey)
{
  delete[] this->appKey;
  this->appKey = appKey;
}


