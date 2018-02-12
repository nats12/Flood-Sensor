/*
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

/*
 * Processor constructor
 * Sets default device settings values (delayPeriod etc...)
 * @param {Sensor} {*sensor} pointer to Sensor object.
 * @param {SDCard} {*sdCard} pointer to SDCard object.
 * @param {Lorawan} {*lorawan} pointer to Lorawan object.
 * @param {byte} {ledPin} LED pin number
 * @param {byte} {interruptPin} engineering menu button interrupt pin number
 * @return N/A
 */
Processor::Processor(Sensor *sensor, SDCard *sdCard, Lorawan *lorawan, byte ledPin, byte interruptPin)
{
  this->sensor = sensor;
  this->sdCard = sdCard;
  this->lorawan = lorawan;
  
  this->ledPin = ledPin;
  this->interruptPin = interruptPin;
  
  this->state = LOW;
  this->delayPeriod = 5000;
  this->delayPeriodARMode = 1000;
  this->ARModeActivationThreshold = 20000; //Threshold (mm) to trigger Accelerated Readings mode
                                     //defaulted to a value that should never trigger (20 meters - out of range of sensor)
  this->ignoreThreshold = 0; //Threshold (mm) for which the sensor should ignore readings - don't send any info to server
}

// State Loop functions

/*
 * Initialise default sensor settings and values
 * Takes initial river depth input from engineer user from serial input to populate other defaults.
 * @param N/A
 * @return {void} N/A
 */
void Processor::init()
{
    //lorawan->join();
  
    String inStr;
    int16_t initialDistanceToRiverTop;
    int16_t currentDistanceToRiverTop;

    // Whilst inStr is null, do nothing, skip
    while ((inStr = Serial.readString()) == NULL){}
    initialRiverDepth = inStr.toInt();
    //Serial.println(initialRiverDepth); 
    initialDistanceToRiverTop = analogRead(sensor->analogPin) * 5;
    sensor->distanceToRiverBed = initialRiverDepth + initialDistanceToRiverTop;

    Serial.println("Current Measurement: ");
    Serial.println(sensor->getCurrentMeasurement());
}

/*
 * Calculates and returns current battery voltage value.
 * @param N/A
 * @return {float} returns measuredvbat value.
 */
float Processor::getBatteryVoltage()
{
  float measuredvbat = analogRead(VBATPIN);
  //** Get a few samples, and smooth values to output (other influences can affect the voltage) e.g. over a period of 10 seconds **
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  return measuredvbat;
}

/*
 * Calculates and returns battery percentage (powerlevel) based on voltage.
 * @param N/A
 * @return {uint8_t} return battery percentage.
 */
uint8_t Processor::getPowerLevel()
{
  return (uint8_t) ((getBatteryVoltage() - 3.2) * 100);
}

/*
 * Process current river depth measurement, and send if relevant.
 * @param N/A
 * @return {void} N/A
 */
void Processor::readingProcess()
{
  int16_t currentRiverLevel = sensor->getCurrentMeasurement();
  
  if(sensor->isCurrentWorthSending(currentRiverLevel))
  { 
    sdCard->printToLog(currentRiverLevel);
    ttn_response_t status = lorawan->sendReading(currentRiverLevel, getPowerLevel());

    //Log error in SDCard log
    if(status != TTN_ERROR_SEND_COMMAND_FAILED) {
      sensor->lastMeasurementSent = currentRiverLevel;
      sdCard->printToLog(currentRiverLevel);
    }
  } else {
    ttn_response_t status = lorawan->sendStillAlive(getPowerLevel());
  }
}

// Accelerated Readings (AR) Mode

/*
 * Set delay period to be used whilst the device is in AR (Accelerated Readings) Mode.
 * @param {int16_t} {newDelayPeriod} new delay period to be used.
 * @return {void} N/A
 */
void Processor::adjustARModeDelay(int16_t newDelayPeriod) //adjust accelerated readings mode with new delay period
{
  delayPeriodARMode = newDelayPeriod;
}

/*
 * Set max river depth threshold to trigger AR Mode (Measured in: mm).
 * @param {int16_t} {newActivationThreshold}  new maximum threshold to trigger AR mode.
 * @return {void} N/A
 */
void Processor::adjustARModeThreshold(int16_t newActivationThreshold)
{
  ARModeActivationThreshold = newActivationThreshold;
}

/*
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
}

/*
 * Set minimum river depth threshold (anything below this value will be ignored, and considered "not worth" sending) (mm).
 * @param {int16_t} {newIgnoreThreshold} new minimum river depth threshold to be set (mm).
 * @return {void} N/A
 */
void Processor::adjustIgnoreThreshold(int16_t newIgnoreThreshold)
{ 
  ignoreThreshold = newIgnoreThreshold;
}


// Helpers
/* 
 * 
 * @param N/A
 * @return {void} N/A
 */
void Processor::writeStatus()
{
  digitalWrite(this->ledPin, this->state);
}

/*
 * 
 * @param N/A
 * @return {void} N/A
 */
void Processor::delayWithPeriod()
{
  char currentMeasurementPeriodMessage[] PROGMEM = "Current measurement period is..";
  Serial.println(currentMeasurementPeriodMessage);
  
  Serial.println(this->delayPeriod);
  delay(this->delayPeriod);
}

// Setters

/*
 * Set the delay period between each measurement to be taken.
 * @param {int16_t} {minutes} new delay period (in minutes) set.
 * @return {void} N/A
 */
void Processor::changeMeasurementPeriod(int16_t minutes)
{
    this->delayPeriod = minutes * 1000;
}

/*
 * Call SDCard class function to store the last measurement sent to a log on the SD card.
 * @param {int16_t} {lastMeasurementSent} last measurement that was sent to the api sucessfully.
 * @return {void} N/A
 */
void Processor::printToSDLog(int16_t lastMeasurementSent)
{
  this->sdCard->printToLog(lastMeasurementSent);
}

/*
 * Call SDCard class function to store the current river depth measurement to a log on the SD card.
 * @param {int16_t} {currentMeasurement} current measurement value to be stored in the SD card log.
 * @return {void} N/A
 */
void Processor::printCurrentMeasurementToSD(int16_t currentMeasurement)
{
  this->sdCard->printCurrentMeasurement(currentMeasurement);
}


