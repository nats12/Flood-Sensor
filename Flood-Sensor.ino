// #include "Ticker-master/Ticker.h"
#include <SPI.h>
#include <SD.h>
#include "EngineeringMenu.h"
#include "Sensor.h"
#include "SDCard.h"
#include "Processor.h"
#include "Lorawan.h"
#define freqPlan TTN_FP_EU868

const byte sensorPin = 0;
const byte sensorPowerPin = 6;
const byte engineeringMenuJumperPin = 12; //Engineering menu jumper pin


Sensor sensor(sensorPin, sensorPowerPin);
SDCard sdCard;
Lorawan lorawan(Serial1, Serial, freqPlan);
Processor processor(&sensor, &sdCard, &lorawan, engineeringMenuJumperPin);
EngineeringMenu menu(&sensor, &sdCard, &processor, &lorawan);

/**
 * Initialises default values and sets up Serial devices, pinModes and engineering menu interrupt.
 * @param N/A
 * @return {Void} N/A
 */
void setup()
{
  //  Setup serial baud, Serial1 used for LoRaWAN, Serial for USB communication.
  Serial1.begin(19200);
  Serial.begin(115200); 
  
  // Wait for serial to connect
  while(!Serial) {}
  Serial.println("USB Serial Setup");

  while(!Serial1){}
  Serial.println("LoRaWan Serial Setup");

  // If engineering menu jumper has been pulled off
  if(digitalRead(engineeringMenuJumperPin) == HIGH) {
    // Load the engineering menu
    menu.loadEngineeringMenu();
  }
  
  // initialize device
  processor.init();
  Serial.println("Device Initialized");

  pinMode(engineeringMenuJumperPin, INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(engineeringMenuJumperPin), setBringUpMenu, CHANGE);
}




/**
 * Loop function to run indefinately.
 * Waits for delay period before attempting to take a river level measurement.
 * Or triggers engineering menu if interrupt function was triggered.
 * @param N/A
 * @return {Void} N/A
 */
void loop()
{
  Serial.println("file size is: ");
  sdCard.fileSize;
  // If engineering menu jumper has been pulled off

  if(!sdCard.fileSize) {
    Serial.println("NULL");
  }
  
  if(digitalRead(engineeringMenuJumperPin) == HIGH) {
    // Load the engineering menu
    menu.loadEngineeringMenu();
  }
  menu.loadEngineeringMenu();
  
//  processor.readingProcess();

  processor.delayWithPeriod();

}

