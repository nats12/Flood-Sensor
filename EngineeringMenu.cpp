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
  String subMenuOption = "0";
  this->sdCard = sdCard;
  this->sensor = sensor;
  this->processor = processor;
}

void EngineeringMenu::mainMenu(String menuOption)
{
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
      String minutes;
      // Insert a new measurement period
      Serial.println("Insert a new measurement period");
      while((minutes = Serial.readString()) == NULL){};
      this->processor->changeMeasurementPeriod(minutes); // Call function to update global variable period to minutes
   }

   if (menuOption == "8\n") {
    Serial.println("1: Set max water level threshold to trigger \"AR\" mode");
    Serial.println("2: Check and send measurement every X minutes during \"AR\" mode(input minutes)");
    this->subMenuOption = "8\n";
   }
}

void EngineeringMenu::subMenuEight(String menuOption)
{
  if(menuOption == "1\n"){
    String newThreshold;
    Serial.println("Threshold (mm):");
    while(newThreshold = Serial.readString() == NULL){};
    this->processor->adjustARModeThreshold(newThreshold.toInt());
    subMenuOption = "0";
  }

  if(menuOption == "2\n") {
    String newDelay;
    Serial.println("Check and send measurement every X minutes: (input minutes)");
    while(newDelay = Serial.readString() == NULL){};
    this->processor->adjustARModeDelay(newDelay.toInt());
    subMenuOption = "0";
  }
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
  Serial.println("8: Adjust \"Accelerated Readings\" thresholds and periods");
  Serial.println("exit: Exit engineering menu");
  
  String menuOption;
  String minutes;
  
  while((menuOption = Serial.readString()) != "exit\n"){
    mainMenu(menuOption);

    if(this->subMenuOption == "8\n")
    {
      subMenuEight(menuOption);
    }
  };
  
  bringUpMenu = false;
}

