// #include "Ticker-master/Ticker.h"
#include <SPI.h>
#include <SD.h>
#include "Sensor.h"
#include "SDCard.h"
#include "EngineeringMenu.h"
#include "Processor.h"

Processor processor;
SDCard sdCard;
Sensor sensor(&sdCard, &processor);

EngineeringMenu menu(&sdCard, &sensor, &processor);





// Initial depth
int initialRiverDepth;


/**
 * 
 */
void setup()

{
 
  // Outputs information to your serial port
  //  Setup serial
  Serial.begin(9600); 
  
  // Wait for serial to connect
  while (!Serial){}
  
  int currentDistanceToRiverTop;

  // Check for incoming serial data:
  //if (Serial.available() > 0) {
    Serial.println("Set up");

    char initialRiverDepthInput[10];
    String inStr;
    int initialDistanceToRiverTop;

    // Whilst inStr is null, do nothing, skip
    while ((inStr = Serial.readString()) == NULL){}
    initialRiverDepth = inStr.toInt();
    //Serial.println(initialRiverDepth); 
    initialDistanceToRiverTop = analogRead(sensor.analogPin) * 5;
    sensor.distanceToRiverBed = initialRiverDepth + initialDistanceToRiverTop;
  //}

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(processor.interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(processor.interruptPin), setBringUpMenu, CHANGE);

}


/**
 * 
 */
void setBringUpMenu() 
{
  menu.bringUpMenu = true;
}


/**
 * 
 */
void loop()

{

  
  digitalWrite(processor.ledPin, processor.state);

  sensor.startReadingProcess();

  // If button interrupt has been pressed
  if(menu.bringUpMenu) {
    // Load the engineering menu
    menu.loadEngineeringMenu();
  }

  Serial.println("Current measurement period is..");
  Serial.println(processor.delayPeriod);
  delay(processor.delayPeriod);

}
