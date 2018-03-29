/**
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


/**
 * Print details of SD card - memory available, FAT type etc
 * Debugging function to check the contents of the SD card.
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


/** 
 * Print the last measurement sent onto a new line in the SD Card text file.
 * @param {String} {lastMeasurementSent} last measurement value sent successfully, to be logged.
 * @return {bool} True if the lastMeasurementSent was successfully written to the text file. False if it fails to write or if the text file failed to open.
 */
bool SDCard::writeToLog(String lastMeasurementSent)
{

  // Open the text file - only one file can be opened at a time so closing right after is important
  myFile = SD.open("LOGGER.TXT", FILE_WRITE);

  // If the file opened okay
  if (myFile) {
    // If number of bytes written is equal to the size of data passed, it has successfully written
    if(myFile.println(data) == data.length()+2) {
      // Get the file size
      // Filesize accuracy lost (/ 1000) to ensure that the number can be stored in a 32 bit int
      fileSize = myFile.size() / 10000;  
      // Close the file:
      myFile.close();
      return true;
    } 
    return false;
  } else {
    // If the file didn't open, print an error:
    char writingSDFileOpeningErrorMessage[] PROGMEM = "Error opening logger.txt";
    Serial.println(writingSDFileOpeningErrorMessage);
    myFile.close();
    return false;
  }
}



/** 
 * Read the contents of the SD Card text file.
 * @param N/A
 * @return {bool} True if the file was successfully opened for readings, false otherwise.
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


/** 
 * Used to test that the last written value in the SD Card text file can be read.
 * It finds the last value in the text file and compares it to the test string. They should equal the same, as the same test string 
 * is called in the write log function prior to this function call during the engineering menu test.
 * @param {String} {testString}
 * @return {bool} True if the last value read from the text file matches the test string. False otherwise.
 */
bool SDCard::testReadLog(String testString) 
{
  
  // Re-open the file for reading:
  myFile = SD.open("LOGGER.TXT");
  if (myFile) {

    char testStringArray[testString.length()+2];
    
    // Look for the position of the last written value in the text file 
    myFile.seek(myFile.size() - (testString.length()+2));
    
    // Read from the file until there's nothing else in it:
//    while (myFile.available()) {
      myFile.read(testStringArray, testString.length()+2);
//    }

    // If there are no differences when comparing the data array to the test string, it is reading the last written value so return true
    if(strcmp(testStringArray, testString.c_str()) == 0) {
      // Close the file:
      myFile.close();
      return true;
    } 

    // Close the file:
    myFile.close();
    return false;
  } else {
    // If the file didn't open, print an error:
    char readingSDFileOpeningErrorMessage[] PROGMEM = "error opening logger.txt";
    Serial.println(readingSDFileOpeningErrorMessage);
    return false;
  }
}


/**
 * Check whether the text file is about to reach its size limit (7741678351 bytes or just under 7.21 GB)
 * @param N/A
 * @return {bool} True if the text file is about to reach its size limit. False otherwise.
 * 
 */
bool SDCard::fileHasReachedSizeLimit()
{ 
  
  // If the file size is equal to or larger than a number close to the limit 7741678351 bytes (200 bytes under the limit = 40 more readings (5 bytes each line (reading)). 
  // This gives the engineer time to go back out and replace SD card without running the risk of no readings being written.
  // File size accuracy is lost (/ 1000) to ensure that the number can be compared against the 32 bit integer fileSize.
  if(fileSize >= 7741678351 / 10000) {
    return true;
  } else {
    return false;
  }
}


/**
 * During setup, this function initialises the SD Card and its library, and reads the card's file size to store in a class variable. 
 * @param N/A 
 * @return {bool} True if the text file was successfully opened for readings. False otherwise.
 */
bool SDCard::init()
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
    // Filesize accuracy lost (/ 1000) to ensure that the number can be stored in a 32 bit int
    fileSize = myFile.size() / 10000; 
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

