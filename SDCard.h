/*
  SDCard.h - Library for dealing with writing/retrieving data to/from SDCard.
  Created by Natalie Mclaren, December 17, 2017.
*/

#ifndef SDCard_h
#define SDCard_h

#include <SPI.h>
#include <SD.h>

/**
 * 
 */
class SDCard
{
  public:
    File myFile;
    SdVolume volume;
    SdFile root;
    Sd2Card card;
    
    SDCard();
    void checkCardMemory();
    void printCurrentMeasurement(int16_t measurement);
    void printToLog(int16_t lastMeasurementSent);
  private:

};

#endif
