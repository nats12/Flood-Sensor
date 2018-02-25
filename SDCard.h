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
    boolean fileHasReachedSizeLimit();
    boolean initSDCard();
    boolean readLog();
    boolean testReadLog(String data);
    boolean writeToLog(String data);
  private:

};

#endif
