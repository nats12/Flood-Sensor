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
 * Constructor, including all other objects to be accessed.
 * Engineering menu functions that are to be used by an maintenance engineer for the device.
 * @param {Sensor} {*sensor} pointer to Sensor object.
 * @param {SDCard} {*sdCard} pointer to SDCard object.
 * @param {Processor} {*processor} pointer to Processor object.
 * @param {Lorawan} {*lorawan} pointer to Lorawan object.
 * @return N/A
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
 * Print all possible menu options (main menu) to Serial device.
 * @param N/A
 * @return {Void} N/A
 */
void EngineeringMenu::printMainMenuOptions()
{
  char menuMessageSelect[] PROGMEM = "Please select an option:";
  char menuMessage1[] PROGMEM = "1: Print current measurement";
  char menuMessage2[] PROGMEM = "2: Print current battery voltage and percentage (Based 100% max being 4.2V)";
  char menuMessage5[] PROGMEM = "5: Test read/write functionality on SD Card";
  char menuMessage6[] PROGMEM = "6: Print details of SD Card";
  char menuMessage7[] PROGMEM = "7: Change measurement period";
  char menuMessage8[] PROGMEM = "8: Adjust \"Accelerated Readings\" thresholds and periods";
  char menuMessageExit[] PROGMEM = "exit: Exit engineering menu";

  Serial.println(menuMessageSelect);
  Serial.println(menuMessage1);
  Serial.println(menuMessage2);
  Serial.println(menuMessage5);
  Serial.println(menuMessage6);
  Serial.println(menuMessage7);
  Serial.println(menuMessage8);
  Serial.println(menuMessageExit);
}

/*
 * Compare menu input to expected menu to check if it is a valid option in the engineering menu.
 * @param {String} {menuOptionInput} menu option manually entered by the engineer.
 * @param {String} {expectedOption} an possible option that valid in the engineering menu.
 * @return {boolean} return valid option or not (true or false).
 */
boolean EngineeringMenu::checkValidMenuOption(String menuOptionInput, String expectedOption)
{
  if(menuOptionInput == expectedOption) {
    return true;
   }
}


/*
 * Return and print current battery voltage (remaing power left in the battery).
 * @param N/A
 * @return {Void} N/A
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
 * Take serial input from user (menuOption String) and execute chosen option.
 * @param {String} {menuOption} input string typed in from engineer user to choose a menu function to run.
 * @return {Void} N/A
 */
void EngineeringMenu::mainMenu(String menuOption)
{
   if(checkValidMenuOption(menuOption, "1\n")) {
     // Print last sent measurement
     this->sdCard->printCurrentMeasurement(this->sensor->getCurrentMeasurement());
     printMainMenuOptions();
   }

   if(checkValidMenuOption(menuOption, "2\n")) {
     // Print current battery voltage and percentage
     printBatteryVoltage();
     printMainMenuOptions();
   }      

   if(checkValidMenuOption(menuOption, "5\n")) {
      // Test read/write on SD card
      char testSDReadWriteMessage[] PROGMEM = "Testing read/write...";
      Serial.println(testSDReadWriteMessage);
      this->sdCard->printToLog(5);
      printMainMenuOptions();
   }

   if (checkValidMenuOption(menuOption, "6\n")) {
      // Print details of SD card space
      this->sdCard->checkCardMemory();
      printMainMenuOptions();
   }

   if (checkValidMenuOption(menuOption, "7\n")) {
      String minutes;
      // Insert a new measurement period
      char setNewMeasurementPeriodMessage[] PROGMEM = "Insert a new measurement period (Number of minutes)";
      char showNewMeasurementPeriodMessage[] PROGMEM = "New measurement delay period (min):";
      Serial.println(setNewMeasurementPeriodMessage);
      
      while((minutes = Serial.readString()) == NULL){};
      this->processor->changeMeasurementPeriod(minutes.toInt()); // Call function to update global variable period to minutes
      Serial.print(showNewMeasurementPeriodMessage);
      Serial.println(processor->delayPeriod / 1000);
      printMainMenuOptions();
   }

   if (checkValidMenuOption(menuOption, "8\n")) {
    
      char setARModeThresholdMessage[] PROGMEM = "1: Set max water level threshold to trigger \"AR\" mode";
      char setARModeSamplingPeriodMessage[] PROGMEM = "2: Check and send measurement every X minutes during \"AR\" mode(input minutes)";
      char returnToMainMenuMessage[] PROGMEM = "r: Return to main menu";
      
      Serial.println(setARModeThresholdMessage);
      Serial.println(setARModeSamplingPeriodMessage);
      Serial.println(returnToMainMenuMessage);
      
      this->subMenuOption = "8\n";
   }

   // Set the Spreading Factor to use for LoRaWAN
   if (checkValidMenuOption(menuOption, "12\n")) {

      char setNewSpreadFactorMessage[] PROGMEM = "Set new spreading factor between 7 and 12";
      
      Serial.println(setNewSpreadFactorMessage);
      
      while((input = Serial.readString()) == NULL){};
      
      lorawan->setSpreadFactor(input.toInt());
   }

   // Set the App Eui used for LoRaWAN
   if (checkValidMenuOption(menuOption, "13\n")) {

      char setAppEuiMessage[] PROGMEM = "Set the App Eui";
      Serial.println(setAppEuiMessage);
      
      while((input = Serial.readString()) == NULL){};
      
      char chArr[25];
      input.toCharArray(chArr, 25);
      lorawan->setAppEui(chArr);
   }

   // Set the App Key used for LoRaWAN
   if (checkValidMenuOption(menuOption, "14\n")) {

      char setAppKeyMessage[] PROGMEM = "Set the App Key";
      Serial.println(setAppKeyMessage);
      
      while((input = Serial.readString()) == NULL){};

      char chArr[25];
      input.toCharArray(chArr, 25);
      lorawan->setAppKey(chArr);
   }
}

/*
 * Show sub-menu options for Main menu option 8.
 * Call function based on user input (menuOption String).
 * @param {String} {menuOption} input string typed in from engineer user to choose a menu function to run. 
 * @return {void} N/A
 */
void EngineeringMenu::subMenuEight(String menuOption)
{
  if(checkValidMenuOption(menuOption, "1\n")){
    String newThreshold;

    char thresholdMessage[] PROGMEM = "Threshold (mm):";
    char cancelMessage[] PROGMEM = "Or type \"r\" to cancel";
    
    Serial.println(thresholdMessage);
    Serial.println(cancelMessage);
    while(newThreshold = Serial.readString() == NULL){};
    if(newThreshold == "r\n"){ //Check if wanting to return to previous menu
      subMenuOption = "0\n"; 
      printMainMenuOptions();
    }
    else {
      this->processor->adjustARModeThreshold(newThreshold.toInt());
      subMenuOption = "0\n";
      printMainMenuOptions();
    }
  }

  if(checkValidMenuOption(menuOption, "2\n")) {
    String newDelay;

    char setSamplePeriodMessage[] PROGMEM = "Check and send measurement every X minutes: (input minutes)";
    Serial.println(setSamplePeriodMessage);
    
    while(newDelay = Serial.readString() == NULL){};
    if(newDelay == "r\n"){ //Check if wanting to return to previous menu
      subMenuOption = "0\n"; 
      printMainMenuOptions();
    }
    else {
      this->processor->adjustARModeDelay(newDelay.toInt());
      subMenuOption = "0\n";
      printMainMenuOptions();
    }
  }

  if(menuOption == "r\n"){ //Check if wanting to return to previous menu
    subMenuOption = "0\n";
    printMainMenuOptions();
  }
}
 
/*
 * Print engineering menu options to Serial device for the engineer user to view.
 * Use serial input (from the engineer user) to select menu options functions.
 * @param N/A
 * @return {void} N/A
 */
void EngineeringMenu::loadEngineeringMenu() 
{  
  char loadingMessage[] PROGMEM = "Loading engineering menu...";
  char menuExitingMessage[] PROGMEM = "You have left the engineering menu, and the device will continue normal operation.";
  String menuOption;
  String minutes;

  Serial.println(loadingMessage);
  printMainMenuOptions();  
  while((menuOption = Serial.readString()) != "exit"){
    mainMenu(menuOption);

    if(this->subMenuOption == "8\n")
    {
      subMenuEight(menuOption);
    }
  };
  Serial.println(menuExitingMessage);
  
  bringUpMenu = false;
}




