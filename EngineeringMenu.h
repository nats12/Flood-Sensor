/*
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
    void mainMenu(String menuOption);
    void subMenuEight(String menuOption);
    void loadEngineeringMenu();
    void printBatteryVoltage();
    
    // Interrupt variable to load engineering menu
    volatile bool bringUpMenu;
  private:
    Sensor *sensor;
    SDCard *sdCard;
    Processor *processor;
    Lorawan *lorawan; 
    String subMenuOption;
    String input;

};

#endif
