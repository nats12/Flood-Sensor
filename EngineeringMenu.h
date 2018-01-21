/*
  EngineeringMenu.h - Library for dealing with the engineering menu.
  Created by Natalie Mclaren, December 17, 2017.
*/


#ifndef EngineeringMenu_h
#define EngineeringMenu_h
#include "SDCard.h"
#include "Sensor.h"
#include "Processor.h"
#include "Arduino.h"

/**
 * 
 */
class EngineeringMenu
{  
  SDCard *sdCard;
  Sensor *sensor;
  Processor *processor;
  
  public:
    EngineeringMenu(SDCard *sdCard, Sensor *sensor, Processor *processor);
    void mainMenu(String menuOption);
    void subMenuEight(String menuOption);
    void loadEngineeringMenu();
    // Interrupt variable to load engineering menu
    volatile bool bringUpMenu;
    
  private:
    String subMenuOption;

};

#endif
