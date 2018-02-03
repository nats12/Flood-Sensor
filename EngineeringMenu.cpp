/*
  EngineeringMenu.h - Library for dealing with the engineering menu.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Arduino.h"
#include "EngineeringMenu.h"
#include "SDCard.h"
#include "Sensor.h"
#include "Processor.h"
#include "EngineeringMenuOptions.h"

/**
 * 
 */
EngineeringMenu::EngineeringMenu(Sensor *sensor, SDCard *sdCard, Processor *processor, EngineeringMenuOptions *options, Lorawan *lorawan)
{
  this->sensor = sensor;
  this->sdCard = sdCard;
  this->processor = processor;
  this->lorawan = lorawan;
  this->options = options;
  
  bringUpMenu = false;
  subMenuOption = "0";
}


/*
 * Return and print current battery voltage (remaing power left in the battery)
 */
void EngineeringMenu::printBatteryVoltage()
{
  //Plot voltage against its remaining capacity - check datasheets
  //Pack into 2 bytes voltage[0], voltage[1]
  //Maybe process this only for the engineering menu, just send voltage to api
  float measuredBatteryVoltage = this->processor->getBatteryVoltage();

  char currentBatteryVoltageMessage[] PROGMEM = "Current battery voltage: ";
  char remainingBatteryPercentageMessage[] PROGMEM = "Remaining battery %";
  
  Serial.print(currentBatteryVoltageMessage); 
  Serial.println(measuredBatteryVoltage);
  Serial.print(remainingBatteryPercentageMessage); //Based on 100% being 4.2V
  Serial.println(this->processor->getPowerLevel());
}

/*
 * 
 */
void EngineeringMenu::mainMenu(String menuOption)
{
   if(this->options->menuOptionOne(menuOption)) {
     // Print last sent measurement
     this->sdCard->printCurrentMeasurement(this->sensor->getCurrentMeasurement());
   }

   if(this->options->menuOptionTwo(menuOption)) {
     // Print current battery voltage and percentage
     printBatteryVoltage();
   }      

   if(this->options->menuOptionFive(menuOption)) {
      // Test read/write on SD card
      char testSDReadWriteMessage[] PROGMEM = "Testing read/write...";
      Serial.println(testSDReadWriteMessage);
      
      this->sdCard->printToLog(5);
   }

   if (this->options->menuOptionSix(menuOption)) {
      // Print details of SD card space
      this->sdCard->checkCardMemory();
   }

   if (this->options->menuOptionSeven(menuOption)) {
      String minutes;
      // Insert a new measurement period
      char setNewMeasurementPeriodMessage[] PROGMEM = "Insert a new measurement period";
      Serial.println(setNewMeasurementPeriodMessage);
      
      while((minutes = Serial.readString()) == NULL){};
      this->processor->changeMeasurementPeriod(minutes.toInt()); // Call function to update global variable period to minutes
   }

   if (this->options->menuOptionEight(menuOption)) {
    
      char setARModeThresholdMessage[] PROGMEM = "1: Set max water level threshold to trigger \"AR\" mode";
      char setARModeSamplingPeriodMessage[] PROGMEM = "2: Check and send measurement every X minutes during \"AR\" mode(input minutes)";
      char returnToMainMenuMessage[] PROGMEM = "r: Return to main menu";
      
      Serial.println(setARModeThresholdMessage);
      Serial.println(setARModeSamplingPeriodMessage);
      Serial.println(returnToMainMenuMessage);
      
      this->subMenuOption = "8\n";
   }

   // Set the Spreading Factor to use for LoRaWAN
   if (this->options->menuOptionTwelve(menuOption)) {

      char setNewSpreadFactorMessage[] PROGMEM = "Set new spreading factor between 7 and 12";
      
      Serial.println(setNewSpreadFactorMessage);
      
      while((input = Serial.readString()) == NULL){};
      
      lorawan->setSpreadFactor(input.toInt());
   }

   // Set the App Eui used for LoRaWAN
   if (this->options->menuOptionThirteen(menuOption)) {

      char setAppEuiMessage[] = "Set the App Eui";
      Serial.println(setAppEuiMessage);
      
      while((input = Serial.readString()) == NULL){};
      
      char chArr[25];
      input.toCharArray(chArr, 25);
      lorawan->setAppEui(chArr);
   }

   // Set the App Key used for LoRaWAN
   if (this->options->menuOptionFourteen(menuOption)) {

      char setAppKeyMessage[] PROGMEM = "Set the App Key";
      Serial.println(setAppKeyMessage);
      
      while((input = Serial.readString()) == NULL){};

      char chArr[25];
      input.toCharArray(chArr, 25);
      lorawan->setAppKey(chArr);
   }
}

void EngineeringMenu::subMenuEight(String menuOption)
{
  if(this->options->menuOptionOne(menuOption)){
    String newThreshold;

    char thresholdMessage[] PROGMEM = "Threshold (mm):";
    char cancelMessage[] PROGMEM = "Or type \"r\" to cancel";
    
    Serial.println(thresholdMessage);
    Serial.println(cancelMessage)
    ;
    while(newThreshold = Serial.readString() == NULL){};
    if(newThreshold == "r/n"){
      subMenuOption = "0"; 
    }
    else {
      this->processor->adjustARModeThreshold(newThreshold.toInt());
      subMenuOption = "0";
    }
  }

  if(this->options->menuOptionTwo(menuOption)) {
    String newDelay;

    char setSamplePeriodMessage[] PROGMEM = "Check and send measurement every X minutes: (input minutes)";
    Serial.println(setSamplePeriodMessage);
    
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
  char loadingMessage[] PROGMEM = "Loading engineering menu...";
  char menuMessageSelect[] PROGMEM = "Please select an option:";
  char menuMessage1[] PROGMEM = "1: Print current measurement";
  char menuMessage2[] PROGMEM = "2: Print current battery voltage and percentage (Based 100% max being 4.2V)";
  char menuMessage5[] PROGMEM = "5: Test read/write functionality on SD Card";
  char menuMessage6[] PROGMEM = "6: Print details of SD Card";
  char menuMessage7[] PROGMEM = "7: Change measurement period";
  char menuMessage8[] PROGMEM = "8: Adjust \"Accelerated Readings\" thresholds and periods";
  char menuMessageExit[] PROGMEM = "exit: Exit engineering menu";


  Serial.println(loadingMessage);
  Serial.println(menuMessageSelect);
  Serial.println(menuMessage1);
  Serial.println(menuMessage2);
  Serial.println(menuMessage5);
  Serial.println(menuMessage6);
  Serial.println(menuMessage7);
  Serial.println(menuMessage8);
  Serial.println(menuMessageExit);
  
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




