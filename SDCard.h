/*
  SDCard.h - Library for dealing with writing/retrieving data to/from SDCard.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include <SPI.h>
#include <SD.h>
#ifndef SDCard_h
#define SDCard_h
#include "Arduino.h"

/**
 * 
 */
class SDCard
{
  
  public:
    SDCard();
    void checkCardMemory();
    void printCurrentMeasurement(int currentMeasurement);
    void printToLog(int lastMeasurementSent);

    File myFile;
    SdVolume volume;
    SdFile root;
    Sd2Card card;
  private:

};

#endif
