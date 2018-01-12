/*
  EngineeringMenu.h - Library for dealing with the engineering menu.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include <SPI.h>
#include <SD.h>
#include "Arduino.h"
#include "EngineeringMenu.h"
#include "SDCard.h"
#include "Sensor.h"
#include "Processor.h"


/**
 * 
 */
EngineeringMenu::EngineeringMenu(SDCard *sdCard, Sensor *sensor, Processor *processor)
{
  
  bringUpMenu = false;
  this->sdCard = sdCard;
  this->sensor = sensor;
  this->processor = processor;
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
  
  String menuOption;
  String minutes;
  
  while((menuOption = Serial.readString()) != "99\n"){
    
   if(menuOption == "1\n") {
     // Print last sent measurement
     this->sdCard->printCurrentMeasurement(this->sensor->getCurrentMeasurement());
   }

   if(menuOption == "5\n") {
      // Test read/write on SD card
      Serial.println("Testing read/write...");
      this->sdCard->printToLog(5);
   }

   if (menuOption == "6\n") {
      // Print details of SD card space
      this->sdCard->checkCardMemory();
   }

   if (menuOption == "7\n") {
      // Insert a new measurement period
      Serial.println("Insert a new measurement period");
      
      while((minutes = Serial.readString()) == NULL){};

      // Call function to update global variable period to minutes
      this->processor->changeMeasurementPeriod(minutes);
   }
  
  };
  
  bringUpMenu = false;
}

