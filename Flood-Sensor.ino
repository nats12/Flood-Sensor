#include "Ticker-master/Ticker.h"


// Connect Pin 3 of the maxbotix to A0.
int analogPin = 0; 

// Initial depth
int initialRiverDepth;
int rangeDifferenceThreshold = 50;
int distanceToRiverBed;
int lastMeasurementSent;
int currentRiverLevel;

volatile bool tickTakeMeasurement = false;


void startReadingProcess();

Ticker tick;


void setup()

{
 
  // Outputs information to your serial port
  //  Setup serial
  Serial.begin(9600); 

  tick.setCallback(startReadingProcess);
  tick.setInterval(2000);
  
  // Start the ticker.
  tick.start(); 
  
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
  }
  //else ignore current measurement
  
}

void setStartReadingProcess()
{
  tickTakeMeasurement = true;
}

void startReadingProcess() {

  currentRiverLevel = getCurrentMeasurement();
  printCurrentMeasurement();
  attemptToSendMeasurement(currentRiverLevel);
}

// Engineering menu
void loadEngineeringMenu() {

  Serial.println("Loading engineering menu...");
  
  int menuOption = Serial.readString().toInt();
  
  if(menuOption == 1) {
    printCurrentMeasurement();
  } else if (menuOption == 2) {
    Serial.println("Do something else");
  }
}

void loop()

{
  if(tickTakeMeasurement)
  {
    startReadingProcess();
  }
  tick.update();
  // sleep();
}
  
  // attachInterrupt(Serial, loadEngineeringMenu, HIGH);
  
//  if(Serial) {
//    // Engineering menu set up
//    loadEngineeringMenu();
//  }
