/**
  EngineeringMenu.h - Library for dealing with the engineering menu.
  Created by Natalie Mclaren, December 17, 2017.
*/

#ifndef EngineeringMenu_h
#define EngineeringMenu_h

#include "SDCard.h"
#include "Sensor.h"
#include "Processor.h"
#include "Lorawan.h"

/**
 * 
 */
class EngineeringMenu
{
  public:
    EngineeringMenu(Sensor *sensor, SDCard *sdCard, Processor *processor, Lorawan *lorawan);
    bool mainMenu(String menuOption);
    void subMenuEight(String menuOption);
    void loadEngineeringMenu();
    void printLoadingEngineeringMenuBox(String loadingMessage);
    void printMainMenuOptions();
    void printBatteryVoltage();
    bool checkValidMenuOption(String menuOptionInput, String expectedOption);
    
    // Interrupt variable to load engineering menu
    bool volatile  bringUpMenu;
  private:
    Sensor *sensor;
    SDCard *sdCard;
    Processor *processor;
    Lorawan *lorawan; 
    String subMenuOption;
    String input;

};

#endif
