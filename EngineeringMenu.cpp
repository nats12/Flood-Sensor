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
EngineeringMenu::EngineeringMenu(Sensor *sensor, SDCard *sdCard, Processor *processor, Lorawan *lorawan)
{
  this->sensor = sensor;
  this->sdCard = sdCard;
  this->processor = processor;
  this->lorawan = lorawan;
  
  bringUpMenu = false;
  subMenuOption = "0";
}

/*
 * Return and print current battery voltage (remaing power left in the battery)
 */
void EngineeringMenu::printBatteryVoltage()
{
  Serial.print("Current battery voltage: " ); 
  Serial.println(this->processor->getBatteryVoltage());
  Serial.print("Remaining battery %"); //Based on 100% being 4.2V
  Serial.println(this->processor->getPowerLevel());
}

/*
 * 
 */
void EngineeringMenu::mainMenu(String menuOption)
{
   if(menuOption == "1\n") {
     // Print last sent measurement
     this->sdCard->printCurrentMeasurement(this->sensor->getCurrentMeasurement());
   }

   if(menuOption == "2\n") {
     // Print current battery voltage and percentage
     printBatteryVoltage();
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
      this->processor->changeMeasurementPeriod(minutes.toInt()); // Call function to update global variable period to minutes
   }

   if (menuOption == "8\n") {
      Serial.println("1: Set max water level threshold to trigger \"AR\" mode");
      Serial.println("2: Check and send measurement every X minutes during \"AR\" mode(input minutes)");
      Serial.println("r: Return to main menu");
      this->subMenuOption = "8\n";
   }

   // Set the Spreading Factor to use for LoRaWAN
   if (menuOption == "12\n") {
      Serial.println("Set new spreading factor between 7 and 12");
      
      while((input = Serial.readString()) == NULL){};
      
      lorawan->setSpreadFactor(input.toInt());
   }

   // Set the App Eui used for LoRaWAN
   if (menuOption == "13\n") {
      Serial.println("Set the App Eui");
      
      while((input = Serial.readString()) == NULL){};
      
      char chArr[25];
      input.toCharArray(chArr, 25);
      lorawan->setAppEui(chArr);
   }

   // Set the App Key used for LoRaWAN
   if (menuOption == "14\n") {
      Serial.println("Set the App Key");
      
      while((input = Serial.readString()) == NULL){};

      char chArr[25];
      input.toCharArray(chArr, 25);
      lorawan->setAppKey(chArr);
   }
}

void EngineeringMenu::subMenuEight(String menuOption)
{
  if(menuOption == "1\n"){
    String newThreshold;
    
    Serial.println("Threshold (mm):");
    Serial.println("Or type \"r\" to cancel");
    while(newThreshold = Serial.readString() == NULL){};
    if(newThreshold == "r/n"){
      subMenuOption = "0"; 
    }
    else {
      this->processor->adjustARModeThreshold(newThreshold.toInt());
      subMenuOption = "0";
    }
  }

  if(menuOption == "2\n") {
    String newDelay;
    Serial.println("Check and send measurement every X minutes: (input minutes)");
    while(newDelay = Serial.readString() == NULL){};
    if(newDelay == "r/n"){
      subMenuOption = "0"; 
    }
    else {
      this->processor->adjustARModeDelay(newDelay.toInt());
      subMenuOption = "0";
    }
  }

  if(menuOption == "r\n"){
    subMenuOption = "0";
  }
}
 
/*
 * 
 */
void EngineeringMenu::loadEngineeringMenu() 
{
  /*
  char menuMessages[][] PROGMEM = {
    "Loading engineering menu...",
    "Please select an option:",
    "1: Print current measurement",
    "2: Print current battery voltage and percentage (Based 100% max being 4.2V)",
    "5: Test read/write functionality on SD Card",
    "6: Print details of SD Card",
    "7: Change measurement period",
    "8: Adjust \"Accelerated Readings\" thresholds and periods",
    "exit: Exit engineering menu"
  };
  */

  char loadingMessage[] PROGMEM = "Loading engineering menu...";
  char menuMessageSelect[] PROGMEM = "Please select an option:";
  char menuMessage1[] PROGMEM = "1: Print current measurement";
  char menuMessage2[] PROGMEM = "2: Print current battery voltage and percentage (Based 100% max being 4.2V)";
  char menuMessage5[] PROGMEM = "5: Test read/write functionality on SD Card";
  char menuMessage6[] PROGMEM = "6: Print details of SD Card";
  char menuMessage7[] PROGMEM = "7: Change measurement period";
  char menuMessage8[] PROGMEM = "8: Adjust \"Accelerated Readings\" thresholds and periods";
  char menuMessageExit[] PROGMEM = "exit: Exit engineering menu";

  /*
  Serial.println("Loading engineering menu...");
  Serial.println("Please select an option:");
  Serial.println("1: Print current measurement");
  Serial.println("2: Print current battery voltage and percentage (Based 100% max being 4.2V)");
  Serial.println("5: Test read/write functionality on SD Card");
  Serial.println("6: Print details of SD Card");
  Serial.println("7: Change measurement period");
  Serial.println("8: Adjust \"Accelerated Readings\" thresholds and periods");
  Serial.println("exit: Exit engineering menu");
  */
  
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


