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
    int16_t fileSize;
    
    SDCard();
    void checkCardMemory();
    bool fileHasReachedSizeLimit();
    bool init();
    bool readLog();
    bool testReadLog(String data);
    bool writeToLog(String data);
  private:

};

#endif
