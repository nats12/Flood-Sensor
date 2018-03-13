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
 * SDCard constructor
 * @param N/A
 * @return N/A
 */
SDCard::SDCard()
{
  
  
}


/*
 * Print details of SD card (memory available etc..)
 * Debugging function to check the status of the SD card - Is everything working correctly?
 * @param N/A
 * @return {void} N/A
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
 * Print the last measurement sent to a new line in a log file on the SD card.
 * @param {int16_t} {lastMeasurementSent} last measurement value sent successfully, to be logged.
 * @return {void} N/A
 */
bool SDCard::writeToLog(String data)
{

  // Open the text file - only one file can be opened at a time so closing right after is important
  myFile = SD.open("LOGGER.TXT", FILE_WRITE);

  // If the file opened okay
  if (myFile) {
    // Write to it
    myFile.println(data);
    // Get the file size
    fileSize = myFile.size(); 
    // Close the file:
    myFile.close();
    return true;
  } else {
    // If the file didn't open, print an error:
    char writingSDFileOpeningErrorMessage[] PROGMEM = "Error opening logger.txt";
    Serial.println(writingSDFileOpeningErrorMessage);
    myFile.close();
    return false;
  }
}



/*
 * 
 */
bool SDCard::readLog()
{
  // Re-open the file for reading:
  myFile = SD.open("LOGGER.TXT");
  if (myFile) {
    // Read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // Close the file:
    myFile.close();
    return true;
  } else {
    // If the file didn't open, print an error:
    char readingSDFileOpeningErrorMessage[] PROGMEM = "error opening logger.txt";
    Serial.println(readingSDFileOpeningErrorMessage);
    return false;
  }
}


/*
 * 
 */
bool SDCard::testReadLog(String data) 
{
  
  // Re-open the file for reading:
  myFile = SD.open("LOGGER.TXT");
  if (myFile) {

    char dataArray[data.length()];

    Serial.println(dataArray);
    myFile.seek(myFile.size() - (data.length()+2));
    
    // Read from the file until there's nothing else in it:
//    while (myFile.available()) {
      myFile.read(dataArray, data.length());
//    }

    Serial.println(dataArray);
    // Close the file:
    myFile.close();
    
    if(strcmp(dataArray, data.c_str()) == 0) {
      return true;
    } 

    return false;
  } else {
    // If the file didn't open, print an error:
    char readingSDFileOpeningErrorMessage[] PROGMEM = "error opening logger.txt";
    Serial.println(readingSDFileOpeningErrorMessage);
    return false;
  }
}


/*
 * Check whether the text file has reached its size limit (7.21 GB or 7741678551 bytes)
 * @param N/A
 * @return {bool} True if the file has reached its size limit, otherwise false
 * 
 */
bool SDCard::fileHasReachedSizeLimit()
{ 
  
  // If the file size is larger than 7741678551 bytes (7.21GB)
  if(fileSize > 7741678551) {
    return true;
  }

  return false;
}


/*
 * 
 */
bool SDCard::initSDCard()
{

  if (!SD.begin(4)) {
    char SDInitializationFailedMessage[] PROGMEM = "initialization failed!";
    Serial.println(SDInitializationFailedMessage);
  }

  char initSDCardMessage[] PROGMEM = "SDCard initialised";
  
  // Open the text file - only one file can be opened at a time so closing right after is important
  myFile = SD.open("LOGGER.TXT", FILE_READ);

  // If the file opened okay
  if (myFile) {
    fileSize = myFile.size(); 
    // Close the file:
    myFile.close();
    Serial.println(initSDCardMessage);
    return true;
  } else {
    // If the file didn't open, print an error:
    char writingSDFileOpeningErrorMessage[] PROGMEM = "Error opening logger.txt";
    Serial.println(writingSDFileOpeningErrorMessage);
    return false;
  }
  
}

