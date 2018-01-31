/*
  SDCard.h - Library for dealing with writing/retrieving data to/from SDCard.
  Created by Natalie Mclaren, December 17, 2017.
*/

#include "SDCard.h"

File myFile;
SdVolume volume;
SdFile root;
Sd2Card card;

/**
 * 
 */
SDCard::SDCard()
{
  
  
}


/*
 * 
 */
void SDCard::checkCardMemory()
{ 
  char cardInfoMessage[] = "\nPrinting card info...";
  Serial.print(cardInfoMessage);

  if (!card.init(SPI_HALF_SPEED, 4)) {
    char SDfailedInitializationMessage[] PROGMEM = "initialization failed. Things to check:";
    char SDcheckIfInsertedMessage[] PROGMEM = "* is a card inserted?";
    char SDcheckIfWiringCorrectMessage[] PROGMEM = "* is your wiring correct?";
    char SDcheckIfPinCorrectMessage[] PROGMEM = "* did you change the chipSelect pin to match your shield or module?";

    
    Serial.println(SDfailedInitializationMessage);
    Serial.println(SDcheckIfInsertedMessage);
    Serial.println(SDcheckIfWiringCorrectMessage);
    Serial.println(SDcheckIfPinCorrectMessage);
    return;
  } else {

    char SDconfirmWiringAndInsertedMessage[] PROGMEM = "Wiring is correct and a card is present.";
    Serial.println(SDconfirmWiringAndInsertedMessage);
  }
  
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {

    char SDpartitionNotFoundMessage[] PROGMEM = "Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card";
    Serial.println(SDpartitionNotFoundMessage);
    return;
  }

  char SDclustersMessage[] PROGMEM = "Clusters:          ";
  Serial.print(SDclustersMessage);
  Serial.println(volume.clusterCount());

  char SDblocksMultipliedByClusterMessage[] PROGMEM = "Blocks x Cluster:  ";
  Serial.print(SDblocksMultipliedByClusterMessage);
  
  Serial.println(volume.blocksPerCluster());

  char SDtotalBlocksMessage[] PROGMEM = "Total Blocks:      ";
  Serial.print(SDtotalBlocksMessage);
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  char SDvolumeTypeMessage[] PROGMEM = "Volume type is:    FAT";
  Serial.print(SDvolumeTypeMessage);
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)

  char SDKbVolumeSizeMessage[] PROGMEM = "Volume size (Kb):  ";
  Serial.print(SDKbVolumeSizeMessage);
  Serial.println(volumesize);

  char SDMbVolumeSizeMessage[] PROGMEM = "Volume size (Mb):  ";
  Serial.print(SDMbVolumeSizeMessage);
  
  volumesize /= 1024;
  Serial.println(volumesize);

  char SDGbVolumeSizeMessage[] PROGMEM = "Volume size (Gb):  ";
  Serial.print(SDGbVolumeSizeMessage);
  Serial.println((float)volumesize / 1024.0);

  char filesFoundInSDMessage[] PROGMEM = "\nFiles found on the card (name, date and size in bytes): ";
  Serial.println(filesFoundInSDMessage);
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}


/*
 * 
 */
void SDCard::printCurrentMeasurement(int measurement)
{
  char currentMeasurementMessage[] PROGMEM = "Current Measurement: ";
  Serial.println(currentMeasurementMessage);
  Serial.println(measurement);
}


/*
 * 
 */
void SDCard::printToLog(int lastMeasurementSent)
{


  if (!SD.begin(4)) {

    char SDInitializationFailedMessage[] PROGMEM = "initialization failed!";
    Serial.println(SDInitializationFailedMessage);
  }


  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("logger.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.println(lastMeasurementSent);
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    char writingSDFileOpeningErrorMessage[] PROGMEM = "error opening logger.txt";
    Serial.println(writingSDFileOpeningErrorMessage);
  }

  // re-open the file for reading:
  myFile = SD.open("logger.txt");
  if (myFile) {
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    char readingSDFileOpeningErrorMessage[] PROGMEM = "error opening logger.txt";
    Serial.println(readingSDFileOpeningErrorMessage);
  }
}
