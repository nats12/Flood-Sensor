/**
  EngineeringMenu.h - Library for dealing with processor settings i.e. its interrupt/led pins.
  Created by Natalie Mclaren, December 18, 2017.
*/
#define VBATPIN A7

#include "Processor.h"
#include <TheThingsNetwork.h>
#include <SPI.h>
#include <SD.h>
#include "Arduino.h"
#include "SDCard.h"
#include "Sensor.h"
#include "Lorawan.h"
#include "FlashStorage.h"

//Reserve space for default device settings in flash storage (EEPROM)
FlashStorage(setupDone_FlashStore, boolean);
FlashStorage(distanceToRiverBed_FlashStore, int16_t);
FlashStorage(delayPeriod_FlashStore, int16_t);
FlashStorage(delayPeriodARMode_FlashStore, int16_t);
FlashStorage(ARModeActivationThreshold_FlashStore, int16_t);
FlashStorage(ignoreThreshold_FlashStore, int16_t);
FlashStorage(appEui_FlashStore, String);
FlashStorage(appKey_FlashStore, String);

/**
 * Processor constructor
 * Sets default device settings values (delayPeriod etc...)
 * @param {Sensor} {*sensor} pointer to Sensor object.
 * @param {SDCard} {*sdCard} pointer to SDCard object.
 * @param {Lorawan} {*lorawan} pointer to Lorawan object.
 * @param {byte} {engineeringMenuJumperPin} engineering menu button interrupt pin number
 * @param {int16_t} {delayPeriod} the delay period value between each samples
 * @param {int16_t} {delayPeriodARMode} the accelerated reading mode value (less of a delay than the delay period)
 * @param {int16_t} {ARModeActivationThreshold} the threshold which activates the ARMode
 * @param {int16_t} {ignoreThreshold} threshold (mm) for which the sensor should ignore readings - don't send any info to server
 * @return N/A
 */
Processor::Processor(Sensor *sensor, SDCard *sdCard, Lorawan *lorawan, byte engineeringMenuJumperPin, int16_t delayPeriod, int16_t delayPeriodARMode, int16_t ARModeActivationThreshold, int16_t ignoreThreshold)
{
  this->sensor = sensor;
  this->sdCard = sdCard;
  this->lorawan = lorawan;
  
  this->engineeringMenuJumperPin = engineeringMenuJumperPin;
  
  this->delayPeriod = delayPeriod;
  this->delayPeriodARMode = delayPeriodARMode;
  this->ARModeActivationThreshold = ARModeActivationThreshold; //Threshold (mm) to trigger Accelerated Readings mode
                                     //defaulted to a value that should never trigger (20 meters - out of range of sensor)
  this->ignoreThreshold = ignoreThreshold; //Threshold (mm) for which the sensor should ignore readings - don't send any info to server
  this->ARModeOn = false;
  this->stillHereCount = 0;
}

// State Loop functions

/**
 * Initialise default sensor settings and values
 * Takes initial river depth input from engineer user from serial input to populate other defaults.
 * @param N/A
 * @return {void} N/A
 */
void Processor::init()
{ 
//    lorawan->join();

    sdCard->init();
    sensor->init();

    char setupSensorMessage[] PROGMEM = "Please place sensor within fifteen minutes for setup to complete.";
    
    if (!setupDone_FlashStore.read()) //no settings saved in flash memory - run setup
    {
      String inStr;
      int16_t initialDistanceToRiverTop;
      int16_t currentDistanceToRiverTop;
      char requestAppEuiMessage[] PROGMEM = "Please input appEui for this device: ";
      char requestCurrentDepthMessage[] PROGMEM = "Please input initial river depth measurement (mm): ";
      char initialRiverDepthMessage[] PROGMEM = "Initial river depth set to: ";
      
      // Whilst inStr is null, do nothing, skip
      //Wait for AppEui input
//      Serial.println(requestAppEuiMessage);
//      while ((inStr = Serial.readString()) == NULL){}
//      char chArr[25];
//      inStr.toCharArray(chArr, 25);
//      Serial.print(inStr);
//      lorawan->setCharAppEui(chArr);
      //Wait for initial/current river depth input
      Serial.println(requestCurrentDepthMessage);
      while ((initialRiverDepth = Serial.readString().toInt()) == 0){}
      Serial.print(initialRiverDepthMessage);
      Serial.print(initialRiverDepth);
      Serial.print("\r\n");
      Serial.println(setupSensorMessage);

      // Delay for 15 minutes
//      delay(900000);
      
      // Delay for 5 minutes for device to placed and setup physically
      initialDistanceToRiverTop = analogRead(sensor->sensorAnalogPin) * 5;
      sensor->distanceToRiverBed = initialRiverDepth + initialDistanceToRiverTop;

      //Store default values in flash storage in case device loses power/resets
      appEui_FlashStore.write(inStr);
      distanceToRiverBed_FlashStore.write(sensor->distanceToRiverBed);
      delayPeriod_FlashStore.write(this->delayPeriod);
      delayPeriodARMode_FlashStore.write(this->delayPeriodARMode);
      ARModeActivationThreshold_FlashStore.write(this->ARModeActivationThreshold);
      ignoreThreshold_FlashStore.write(this->ignoreThreshold);
      
      setupDone_FlashStore.write(true);
    }
    else  
    {
      //Read saved default values from flash storage
      sensor->distanceToRiverBed = distanceToRiverBed_FlashStore.read();
      this->delayPeriod = delayPeriod_FlashStore.read();
      this->delayPeriodARMode = delayPeriodARMode_FlashStore.read();
      this->ARModeActivationThreshold = ARModeActivationThreshold_FlashStore.read();
      this->ignoreThreshold = ignoreThreshold_FlashStore.read();

      Serial.println(setupSensorMessage);
      // Delay for 15 minutes
//      delay(900000);
    }
    
    Serial.println("Current Measurement: ");
    Serial.println(sensor->getCurrentMeasurement());
}



/**
 * Calculates and returns current battery voltage value.
 * Gets an average of 5 readings over 5 seconds.
 * @param N/A
 * @return {float} returns measuredvbat value.
 */
float Processor::getBatteryVoltage()
{
  float avgMeasuredVBat;
  float totalMeasuredVBat = 0;
  
  // Get avg of 5 readings over a period of 4 seconds
  totalMeasuredVBat += analogRead(VBATPIN);
  for(uint8_t i = 0; i < 4; i++)
  {
    delay(1000); //wait for 1 second before taking checking voltage again
    totalMeasuredVBat += analogRead(VBATPIN);
  }
  avgMeasuredVBat = totalMeasuredVBat / 5;
  avgMeasuredVBat *= 2;    // we divided by 2, so multiply back
  avgMeasuredVBat *= 3.3;  // Multiply by 3.3V, our reference voltage
  avgMeasuredVBat /= 1024; // convert to voltage
  
  return avgMeasuredVBat;
}

/**
 * Converts battery voltage to a byte to be sent to the API.
 * Done to save TTN bandwidth (So we don't need to send send a full floating point number).
 * @param N/A
 * @return {uint8_t} returns voltage as an interger between 1 and 100 to be then recalculated in the API
 */
uint8_t Processor::getBatteryVoltageByte()
{
//  Serial.println(getBatteryVoltage());
  float voltage = getBatteryVoltage() - 3.2;

  if(voltage > 1.0) {
    voltage = 1.0;
  } else if(voltage > 1.0) {
    voltage = 0;
  }
  
  return floor(voltage * 100);
}

/**
 * Calculates and returns estimated battery percentage (powerlevel/capacity) based on voltage and mAh of the battery.
 * Expected default battery:
 * 4.2v max (and 3.2v min cut off)
 * 700mAh
 * @param N/A
 * @return {uint8_t} return battery percentage.
 */
uint8_t Processor::getEstimatedPowerLevel()
{
  //100% capacity based on 2.94 Watt hours (4.2v and 0.7Ah)
  //Remove usless power below 3.2v (2.94 - 2.24 = 0.7) - MaxCapacityVal
  //percentage = (( (voltage * Ah) -2.24) / MaxCapacityVal) * 100
  uint8_t estimatedPercentage = floor( ( ( ( (getBatteryVoltage() * 0.7) - 2.24) / 0.7) * 100)); // We are using a 700mAh battery
  return estimatedPercentage;
}

/**
 * Process current river depth measurement, and send if relevant.
 * @param N/A
 * @return {void} N/A
 */
void Processor::readingProcess()
{
  int16_t currentRiverLevel = sensor->getCurrentMeasurement();
  
  //Check if it is worth sending and higher than (or equal to) the ignored depth threshold
  if(sensor->isCurrentWorthSending(currentRiverLevel) && currentRiverLevel >= ignoreThreshold) { 
    // If the SDCard file is full
    if(this->sdCard->fileHasReachedSizeLimit()) {
      // Send a storage error
      this->lorawan->sendStorageError(getBatteryVoltageByte());
      char fullSDCardFileMessage[] PROGMEM = "The SDCard seems to be full, error writing to it...";
      Serial.println(fullSDCardFileMessage);
    } else {
      // Log the measurement
      printMeasurementToSDLog(currentRiverLevel);
    }

    ttn_response_t status = lorawan->sendReading(currentRiverLevel, getBatteryVoltageByte());
    
    //Log error in SDCard log
    if(status != TTN_ERROR_SEND_COMMAND_FAILED) {
      sensor->lastMeasurementSent = currentRiverLevel;
      stillHereCount = 0;
    } else if(!this->sdCard->fileHasReachedSizeLimit()) {
      printMeasurementToSDLog(status);
    } 
  } 
  else if (stillHereCount >= 24) {
    ttn_response_t status = lorawan->sendStillAlive(getBatteryVoltageByte());
       
    if(status == TTN_ERROR_SEND_COMMAND_FAILED && !this->sdCard->fileHasReachedSizeLimit()) {
      printMeasurementToSDLog(status);
    } else {
      stillHereCount = 0; 
    }
  } 
  else {
    stillHereCount++;
  }
  
  //Check AR Mode threshold
  if((currentRiverLevel >= ARModeActivationThreshold && !this->ARModeOn) || (currentRiverLevel < ARModeActivationThreshold && this->ARModeOn))
  {
    activateOrDeactivateARMode();
  }
}


/**
 * Used to calibrate the sensor as done in initial setup.
 * Allows the engineer to input an initial depth for the river, for which the distance from sensor to river bed can be calculated.

 * @param {N/A}
 * @return {N/A}
 */
void Processor::recalibrateSensor()
{
  int16_t newCurrentDepth;
  int16_t distanceToRiverTop;
  
  //wait for/get current depth input from engineer
  while ((newCurrentDepth = Serial.readString().toInt()) == 0){};
  distanceToRiverTop = analogRead(sensor->sensorAnalogPin) * 5;
  sensor->distanceToRiverBed = newCurrentDepth + distanceToRiverTop; //river depth + distanceToRiverTop
  distanceToRiverBed_FlashStore.write(sensor->distanceToRiverBed);
}

/**
 * Clear the Flash storage.
 * @param {N/A}
 * @return {Void}
 */
void Processor::triggerClearFlash()
{
  setupDone_FlashStore.write(false);
}


/**
 * Adjust the application's Eui
 * @param {String} {newAppEui} The new Eui value
 * @return {N/A}
 */
void Processor::adjustAppEui(String newAppEui)
{
  char chArr[25];
  newAppEui.toCharArray(chArr, 25);
  lorawan->setCharAppEui(chArr);
  appEui_FlashStore.write(newAppEui);
}


/**
 * Adjust the application's key
 * @param {String} {newAppKey} The new application key value
 * @return {N/A}
 */
void Processor::adjustAppKey(String newAppKey)
{
  char chArr[25];
  newAppKey.toCharArray(chArr, 25);
  lorawan->setAppKey(chArr);
  appKey_FlashStore.write(newAppKey);
}

// Accelerated Readings (AR) Mode

/**
 * Set delay period to be used whilst the device is in AR (Accelerated Readings) Mode.
 * @param {int16_t} {newDelayPeriod} new delay period to be used.
 * @return {void} N/A
 */
void Processor::adjustARModeDelay(int16_t newDelayPeriod) //adjust accelerated readings mode with new delay period
{
  delayPeriodARMode = newDelayPeriod;
  delayPeriodARMode_FlashStore.write(newDelayPeriod);
}

/**
 * Set max river depth threshold to trigger AR Mode (Measured in: mm).
 * @param {int16_t} {newActivationThreshold}  new maximum threshold to trigger AR mode.
 * @return {void} N/A
 */
void Processor::adjustARModeThreshold(int16_t newActivationThreshold)
{
  ARModeActivationThreshold = newActivationThreshold;
  ARModeActivationThreshold_FlashStore.write(newActivationThreshold);
}

/**
 * Swaps delayPeriod and delayPeriodARMode variables to activate/deactivate Accelerated Readings mode.
 * @param N/A
 * @return {void} N/A
 */
void Processor::activateOrDeactivateARMode()
{
  int16_t tempPeriod = 0;
  tempPeriod = delayPeriod;
  delayPeriod = delayPeriodARMode;
  delayPeriodARMode = tempPeriod;
  ARModeOn = !ARModeOn;
}

/**
 * Set minimum river depth threshold (anything below this value will be ignored, and considered "not worth" sending) (mm).
 * @param {int16_t} {newIgnoreThreshold} new minimum river depth threshold to be set (mm).
 * @return {void} N/A
 */
void Processor::adjustIgnoreThreshold(int16_t newIgnoreThreshold)
{ 
  ignoreThreshold = newIgnoreThreshold;
}


// Helpers
/**
 * Makes the entire device sleep for a specified period of time.
 * Used to sleep inbetween taking river depth measurements.
 * @param N/A
 * @return {void} N/A
 */
void Processor::delayWithPeriod()
{
//  char currentMeasurementPeriodMessage[] PROGMEM = "Current measurement period is..";
//  Serial.println(currentMeasurementPeriodMessage);
  
  Serial.println(this->delayPeriod);
  delay(this->delayPeriod);
}

// Setters

/**
 * Set the delay period between each measurement to be taken.
 * @param {int16_t} {minutes} new delay period (in minutes) set.
 * @return {void} N/A
 */
void Processor::changeMeasurementPeriod(int16_t minutes)
{
    this->delayPeriod = (minutes * 1000) * 60;
    delayPeriod_FlashStore.write(this->delayPeriod);
}

/**
 * Call SDCard class function to store the last measurement sent to a log on the SD card.
 * @param {int16_t} {lastMeasurementSent} last measurement that was sent to the api sucessfully.
 * @return {void} N/A
 */
void Processor::printMeasurementToSDLog(int16_t measurement)
{
  this->sdCard->writeToLog(String(measurement));
}


