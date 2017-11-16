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
volatile bool bringUpMenu = false;


void startReadingProcess();

// Ticker tick;
File myFile;


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

// Engineering menu
void loadEngineeringMenu() {

  Serial.println("Loading engineering menu...");
  
  String menuOption;
  
  while((menuOption = Serial.readString()) != "99\n"){
    
   if(menuOption == "1\n") {
     printCurrentMeasurement();
   } 
   
   if (menuOption == "2\n") {
    Serial.println("Do something else");
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

  if(bringUpMenu) {
    loadEngineeringMenu();
  }
  
  delay(2000);

}
