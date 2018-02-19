// #include "Ticker-master/Ticker.h"
#include <SPI.h>
#include <SD.h>
#include "EngineeringMenu.h"
#include "Sensor.h"
#include "SDCard.h"
#include "Processor.h"
#include "Lorawan.h"

const byte ledPin = 1;
const byte interruptPin = 13; //Engineering menu button

Sensor sensor(0);
SDCard sdCard;
Lorawan lorawan(7);
Processor processor(&sensor, &sdCard, &lorawan, ledPin, interruptPin);
EngineeringMenu menu(&sensor, &sdCard, &processor, &lorawan);

/**
 * Initialises default values and sets up Serial devices, pinModes and engineering menu interrupt.
 * @param N/A
 * @return {Void} N/A
 */
void setup()
{
  //  Setup serial baus, Serial1 used for LoRaWAN, Serial for USB communication.
  Serial1.begin(57600);
  Serial.begin(11200); 
  
  // Wait for serial to connect
  while (!Serial){}
  
  // Check for incoming serial data:
  //if (Serial.available() > 0) {
    Serial.println("Set up");

    processor.init();
  //}

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), setBringUpMenu, CHANGE);
}

/**
 * Interrupt function to trigger engineering menu.
 * @param N/A
 * @return {Void} N/A
 */
void setBringUpMenu() 
{
  menu.bringUpMenu = true;
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
  //processor.writeStatus();

  // If button interrupt has been pressed
  if(menu.bringUpMenu) {
    // Load the engineering menu
    menu.loadEngineeringMenu();
  }

  //processor.readingProcess();
  
  //processor.delayWithPeriod();

}
