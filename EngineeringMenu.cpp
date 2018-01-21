/*
  EngineeringMenu.h - Library for dealing with the engineering menu.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Arduino.h"
#include "EngineeringMenu.h"
#include "SDCard.h"
#include "Sensor.h"
#include "Processor.h"

/**
 * 
 */
EngineeringMenu::EngineeringMenu(Sensor *sensor, SDCard *sdCard, TheThingsNetwork *ttn)
{
  this->sensor = sensor;
  this->sdCard = sdCard;
  this->ttn = ttn;
  
  bringUpMenu = false;
}

/*
 * 
 */
void EngineeringMenu::loadEngineeringMenu() 
{

  Serial.println("Loading engineering menu...");
  Serial.println("Please select an option:");
  Serial.println("1: Print current measurement");
  Serial.println("5: Test read/write functionality on SD Card");
  Serial.println("6: Print details of SD Card");
  Serial.println("7: Change measurement period");
  Serial.println("12: Adjust spreading factor");
  Serial.println("13: Set appeui");
  Serial.println("14: Set appkey");
  
  String menuOption;
  String input;
  
  while((menuOption = Serial.readString()) != "99\n"){
    
   if(menuOption == "1\n") {
       // Print last sent measurement
       sdCard->printCurrentMeasurement();
   }

   if(menuOption == "5\n") {
      // Test read/write on SD card
      Serial.println("Testing read/write...");
      sdCard->printToLog(5);
   }

   if (menuOption == "6\n") {
      // Print details of SD card space
      sdCard->checkCardMemory();
   }

   if (menuOption == "7\n") {
      // Insert a new measurement period
      Serial.println("Insert a new measurement period");
      
      while((input = Serial.readString()) == NULL){};

      // Call function to update global variable period to minutes
      sensor->changeMeasurementPeriod(input.toInt());
   }

   // Set the Spreading Factor to use for LoRaWAN
   if (menuOption == "12\n") {
      Serial.println("Set new spreading factor between 7 and 12");
      
      while((input = Serial.readString()) == NULL){};

      processor->setSpreadingFactor(input.toInt());
   }

   // Set the App Eui used for LoRaWAN
   if (menuOption == "13\n") {
      Serial.println("Set the App Eui");
      
      while((input = Serial.readString()) == NULL){};
 
      char chArr[25];
      input.toCharArray(chArr, 25);
      processor->setAppEui(chArr);
   }

   // Set the App Key used for LoRaWAN
   if (menuOption == "14\n") {
      Serial.println("Set the App Key");
      
      while((input = Serial.readString()) == NULL){};

      char chArr[25];
      input.toCharArray(chArr, 25);
      processor->setAppKey(chArr);
   }
  
  };
  
  bringUpMenu = false;
}

