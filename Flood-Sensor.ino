// #include "Ticker-master/Ticker.h"
#include <SPI.h>
#include <SD.h>


const byte ledPin = 1;
const byte interruptPin = 13;
volatile byte state = LOW;

// Connect Pin 3 of the maxbotix to A0.
int analogPin = 0; 

// Initial depth
int initialRiverDepth;
int rangeDifferenceThreshold = 50;
int distanceToRiverBed;
int lastMeasurementSent;
int currentRiverLevel;

volatile bool canStart = false;

// Interrupt variable to load engineering menu
volatile bool bringUpMenu = false;

// Delay period at which it takes readings
int period = 5000;

void startReadingProcess();

// Ticker tick;
File myFile;
SdVolume volume;
SdFile root;
Sd2Card card;

void setup()

{
 
  // Outputs information to your serial port
  //  Setup serial
  Serial.begin(9600); 
  
  // tick.setCallback(startReadingProcess);
  // tick.setInterval(2000);
  
  // Start the ticker.
  // tick.start(); 
  
  // Wait for serial to connect
  while (!Serial){}
  
  int currentDistanceToRiverTop;

  // Check for incoming serial data:
  //if (Serial.available() > 0) {
    Serial.println("Set up");

    char initialRiverDepthInput[10];
    String inStr;
    int initialDistanceToRiverTop;

    // Whilst inStr is null, do nothing, skip
    while ((inStr = Serial.readString()) == NULL){}
    initialRiverDepth = inStr.toInt();
    //Serial.println(initialRiverDepth); 
    initialDistanceToRiverTop = analogRead(analogPin) * 5;
    distanceToRiverBed = initialRiverDepth + initialDistanceToRiverTop;
  //}

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), setBringUpMenu, CHANGE);

}

int getCurrentMeasurement()
{
  // Read a raw value
  int rawVal = analogRead(analogPin);
  // As per datasheet (to get mm)   
  int currentDistanceToRiverTop = rawVal * 5;  
  // Output value  

  return distanceToRiverBed - currentDistanceToRiverTop;
}

bool isCurrentWorthSending(int currentMeasurement)
{
  return (abs(currentMeasurement - lastMeasurementSent)) >= rangeDifferenceThreshold;
}


void checkCardMemory()
{ 
  Serial.print("\nPrinting card info...");

  if (!card.init(SPI_HALF_SPEED, 4)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }
  
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }

  Serial.print("Clusters:          ");
  Serial.println(volume.clusterCount());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(volume.blocksPerCluster());

  Serial.print("Total Blocks:      ");
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is:    FAT");
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial.print("Volume size (Kb):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):  ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb):  ");
  Serial.println((float)volumesize / 1024.0);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}


void printToLog(int lastMeasurementSent)
{


  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
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
    Serial.println("error opening logger.txt");
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
    Serial.println("error opening logger.txt");
  }
}


bool sendMeasurement(int currentMeasurement)
{
  Serial.println("Measurement Sent:");
  Serial.println(currentMeasurement);
  return true;
}

void printCurrentMeasurement()
{
  Serial.println("Current Measurement: ");
  Serial.println(getCurrentMeasurement());
}

void attemptToSendMeasurement(int currentMeasurement)
{
  if(isCurrentWorthSending(currentMeasurement))
  {
    if(sendMeasurement(currentMeasurement))
      lastMeasurementSent = currentMeasurement;
      printToLog(lastMeasurementSent);
  }
  //else ignore current measurement
  
}


void startReadingProcess() {

  currentRiverLevel = getCurrentMeasurement();
  printCurrentMeasurement();
  attemptToSendMeasurement(currentRiverLevel);
}

void changeMeasurementPeriod(String minutes)
{
  // Update the delay period
  period = minutes.toInt();
}



// Engineering menu
void loadEngineeringMenu() 
{

  Serial.println("Loading engineering menu...");
  Serial.println("Please select an option:");
  Serial.println("1: Print current measurement");
  Serial.println("5: Test read/write functionality on SD Card");
  Serial.println("6: Print details of SD Card");
  Serial.println("7: Change measurement period");
  
  String menuOption;
  String minutes;
  
  while((menuOption = Serial.readString()) != "99\n"){
    
   if(menuOption == "1\n") {
     // Print last sent measurement
     printCurrentMeasurement();
   }

   if(menuOption == "5\n") {
      // Test read/write on SD card
      Serial.println("Testing read/write...");
      printToLog(5);
   }

   if (menuOption == "6\n") {
      // Print details of SD card space
      checkCardMemory();
   }

   if (menuOption == "7\n") {
      // Insert a new measurement period
      Serial.println("Insert a new measurement period");
      
      while((minutes = Serial.readString()) == NULL){};

      // Call function to update global variable period to minutes
      changeMeasurementPeriod(minutes);
   }
   
  };
  
  bringUpMenu = false;
}

void setBringUpMenu() 
{
  bringUpMenu = true;
}

void loop()

{

  digitalWrite(ledPin, state);

  startReadingProcess();

  // If button interrupt has been pressed
  if(bringUpMenu) {
    // Load the engineering menu
    loadEngineeringMenu();
  }

  Serial.println("Current measurement period is..");
  Serial.println(period);
  delay(period);

}
