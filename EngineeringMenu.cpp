/*
  EngineeringMenu.h - Library for dealing with the engineering menu.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "Arduino.h"
#include "EngineeringMenu.h"
#include "SDCard.h"
#include "Sensor.h"
#include "Processor.h"
// Needed to be able to use 'Serial << ' syntax. The class needs to be associated with the Print class
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; } 

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
  
  bringUpMenu = true; //true to test
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
  char menuMessage12[] PROGMEM = "12: Set the Spreading Factor to use for LoRaWAN";
  char menuMessage13[] PROGMEM = "13: Set the App Eui used for LoRaWAN";
  char menuMessage14[] PROGMEM = "14: Set the App Key used for LoRaWAN";
  char menuMessage15[] PROGMEM = "15: Get and print the App Eui used for LoRaWAN";
  char menuMessageExit[] PROGMEM = "exit: Exit engineering menu";
  char menuBoxMessage[] PROGMEM = "Engineering Menu";

  printLoadingEngineeringMenuBox(menuBoxMessage);
  
  Serial.println(menuMessageSelect);
  Serial.println(menuMessage1);
  Serial.println(menuMessage2);
  Serial.println(menuMessage5);
  Serial.println(menuMessage6);
  Serial.println(menuMessage7);
  Serial.println(menuMessage8);
  Serial.println(menuMessage12);
  Serial.println(menuMessage13);
  Serial.println(menuMessage14);
  Serial.println(menuMessage15);
  Serial.println(menuMessageExit);
}

/*
 * Compare menu input to expected menu to check if it is a valid option in the engineering menu.
 * @param {String} {menuOptionInput} menu option manually entered by the engineer.
 * @param {String} {expectedOption} an possible option that valid in the engineering menu.
 * @return {bool} return valid option or not (true or false).
 */
bool EngineeringMenu::checkValidMenuOption(String menuOptionInput, String expectedOption)
{
  if(menuOptionInput == expectedOption) {
    return true;
   }
}


/*
 * Return and print current battery voltage (remaing power left in the battery).
 * Also print an estimated battery percentage (capacity remaining)
 * @param N/A
 * @return {Void} N/A
 */
void EngineeringMenu::printBatteryVoltage()
{
  //Plot voltage against its remaining capacity - check datasheets
  float measuredBatteryVoltage = this->processor->getBatteryVoltage();

  char currentBatteryVoltageMessage[] PROGMEM = "Current battery voltage: ";
  char remainingBatteryPercentageMessage[] PROGMEM = "Estimated remaining battery %";
  
  Serial.print(currentBatteryVoltageMessage); 
  Serial.println(measuredBatteryVoltage);
  Serial.print(remainingBatteryPercentageMessage); //Based on 100% being 4.2V
  Serial.println(this->processor->getEstimatedPowerLevel());
}

/*
 * Take serial input from user (menuOption String) and execute chosen option.
 * @param {String} {menuOption} input string typed in from engineer user to choose a menu function to run.
 * @return {Void} N/A
 */
bool EngineeringMenu::mainMenu(String menuOption)
{ 
   
    if(menuOption != "") {
      // If option one 
      if(checkValidMenuOption(menuOption, "1")) {   // If option one
         // Print last sent measurement
         Serial.println(this->sensor->getCurrentMeasurement());
         printMainMenuOptions();
       } else if(checkValidMenuOption(menuOption, "2\n")) {   // If option two
         // Print current battery voltage and percentage
         printBatteryVoltage();
         printMainMenuOptions();
       } else if(checkValidMenuOption(menuOption, "5\n")) {   // If option five
          // Test read/write on SD card
          char testSDReadWriteMessage[] PROGMEM = "Testing read/write...";
          Serial.println(testSDReadWriteMessage);

          // If the file was written to okay
          if(this->sdCard->writeToLog("test string")) {
            char testSDWritePassedMessage[] PROGMEM = "Writing: passed";
            Serial.println(testSDWritePassedMessage);
          } else {
            char testSDWriteFailedMessage[] PROGMEM = "Writing: failed";
            Serial.println(testSDWriteFailedMessage);
          }
          
          // If the file was read ok
          if(this->sdCard->testReadLog("test string")) {
            char testSDReadPassedMessage[] PROGMEM = "Reading: passed";
            Serial.println(testSDReadPassedMessage);
          } else {
            char testSDReadFailedMessage[] PROGMEM = "Reading: failed";
            Serial.println(testSDReadFailedMessage);
          }
          
          // If the SDCard file is full
          if(this->sdCard->fileHasReachedSizeLimit()) {
            // Send a storage error
//            this->lorawan->sendStorageError(getBatteryVoltageByte());
            char fullSDCardFileMessage[] PROGMEM = "The SDCard seems to be full, error writing to it...";
            Serial.println(fullSDCardFileMessage);
          } 
          printMainMenuOptions();
       } else if (checkValidMenuOption(menuOption, "6\n")) {    // If option six
          // Print details of SD card space
          this->sdCard->checkCardMemory();
          printMainMenuOptions();
       } else if (checkValidMenuOption(menuOption, "7\n")) {    // If option seven
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
       } else if (checkValidMenuOption(menuOption, "8\n")) {    // If option eight
        
          char setARModeThresholdMessage[] PROGMEM = "1: Set max water level threshold to trigger \"AR\" mode";
          char setARModeSamplingPeriodMessage[] PROGMEM = "2: Check and send measurement every X minutes during \"AR\" mode(input minutes)";
          char returnToMainMenuMessage[] PROGMEM = "r: Return to main menu";
          
          Serial.println(setARModeThresholdMessage);
          Serial.println(setARModeSamplingPeriodMessage);
          Serial.println(returnToMainMenuMessage);
          
          this->subMenuOption = "8\n";
       } else if (checkValidMenuOption(menuOption, "12\n")) {   // If option twelve
          // Set the Spreading Factor to use for LoRaWAN
          char setNewSpreadFactorMessage[] PROGMEM = "Set new spreading factor between 7 and 12";
          
          Serial.println(setNewSpreadFactorMessage);
          
          while((input = Serial.readString()) == NULL){};
          
          lorawan->setSpreadFactor(input.toInt());
       } else if (checkValidMenuOption(menuOption, "13\n")) {   // If option thirteen
          // Set the App Eui used for LoRaWAN
          char setAppEuiMessage[] PROGMEM = "Set the App Eui";
          Serial.println(setAppEuiMessage);
          
          while((input = Serial.readString()) == NULL){};
          
          char chArr[25];
          input.toCharArray(chArr, 25);
          lorawan->setCharAppEui(chArr);
       } else if (checkValidMenuOption(menuOption, "14\n")) { // If option fourteen
         // Set the App Key used for LoRaWAN
          char setAppKeyMessage[] PROGMEM = "Set the App Key";
          Serial.println(setAppKeyMessage);
          
          while((input = Serial.readString()) == NULL){};
    
          char chArr[25];
          input.toCharArray(chArr, 25);
          lorawan->setAppKey(chArr);
        } else if (checkValidMenuOption(menuOption, "15\n")) { // If option fourteen
         // Set the App Key used for LoRaWAN
          char getDevEuiMessage[] PROGMEM = "Get the Dev Eui";
          Serial.println(getDevEuiMessage);

          char bufR[20];
          lorawan->showStatus();
          
//          Serial.println(lorawan->getHardwareEui(bufR, 20));
       } else if(checkValidMenuOption(menuOption, "exit\n")) {    // If option is exit
        char menuExitingMessage[] PROGMEM = "You have left the engineering menu, and the device will continue normal operation.";
        Serial.println(menuExitingMessage);
      
        bringUpMenu = false;
      } else {    // If the menu option was invalid (none of the above)
        // Display an error message
        char invalidMenuOptionMessage[] PROGMEM = "Invalid menu option, please try again.";
        Serial.println(invalidMenuOptionMessage);
      }
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
 * Print a box around the engineering menu 'loading' message
 * @param {String} {loadingMessage} The loading message 
 * @return {void} N/A
 */
void EngineeringMenu::printLoadingEngineeringMenuBox(String loadingMessage)
{
  // (top line) loadingMessage size plus 4 to accommodate speech marks and spaces
    for(int i = 0; i < loadingMessage.length() + 4; i++) {
        Serial << "*";
    }
      Serial << "\r\n";

    // (sides & message)
    for(int i = 0; i < 1; i++) {
        Serial<<"* " << loadingMessage << " *" << "\r\n";
    }

    // (bottom line) loadingMessage size plus 4 to accommodate speech marks and spaces
    for(int i = 0; i < loadingMessage.length() + 4; i++) {
        Serial << "*";
   }
      Serial << "\r\n";
}



/*
 * Print engineering menu options to Serial device for the engineer user to view.
 * Use serial input (from the engineer user) to select menu options functions.
 * @param N/A
 * @return {void} N/A
 */
void EngineeringMenu::loadEngineeringMenu() 
{  
  String loadingMessage = "Loading engineering menu...";
  String menuOption;
  String minutes;

    
  printLoadingEngineeringMenuBox(loadingMessage);
  printMainMenuOptions();

  // While the option is not empty
  while((menuOption = Serial.readString()) != " " && bringUpMenu) {
    
    // Run with the entered option
    mainMenu(menuOption);
      
    if(this->subMenuOption == "8\n")
    {
      subMenuEight(menuOption);
    }
  }; 
}




