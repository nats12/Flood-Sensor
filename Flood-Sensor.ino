#include "Ticker-master/Ticker.h"
#include <TheThingsNetwork.h>
#define loraSerial Serial1
#define freqPlan TTN_FP_EU868

//Keys for Lorawan
const char *appEui = "70B3D57EF00000C3";
const char *appKey = "9219CFB30320D36373218DB4EF00D1CB";

//TTN class instance
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

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
  // Setup debug serial
  Serial.begin(9600);
  // Setup Lorawan serial
  loraSerial.begin(57600); 

  // Wait for serial to connect
  while (!Serial){}
  Serial.print("starting up");

  // Join the things network
  if(!ttn.join(appEui, appKey)) {
    // if returned false error
    Serial.print("Couldn't connect to TTN");
  }
  
  // Setup ticker
  tick.setCallback(startReadingProcess);
  tick.setInterval(10000);
  
  // Start the ticker.
  tick.start(); 
    
  int16_t currentDistanceToRiverTop;

  // Check for incoming serial data:
  //if (Serial.available() > 0) {
    Serial.println("Set up");

    char initialRiverDepthInput[10];
    String inStr;
    int16_t initialDistanceToRiverTop;

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
  int16_t currentDistanceToRiverTop = rawVal * 5;  
  // Output value  

  return distanceToRiverBed - currentDistanceToRiverTop;
}

bool isCurrentWorthSending(int16_t currentMeasurement)
{
  return (abs(currentMeasurement - lastMeasurementSent)) >= rangeDifferenceThreshold;
}


bool sendMeasurement(int16_t currentMeasurement)
{
  byte data[3];
  data[0] = 0;
  // Work on getting powerlevel
  data[1] = highByte(currentMeasurement);
  data[2] = lowByte(currentMeasurement);

  ttn.sendBytes(data, sizeof(data));

  Serial.println("Measurement Sent:");
  Serial.println(currentMeasurement);
  return true;
}

void printCurrentMeasurement()
{
  Serial.println("Current Measurement: ");
  Serial.println(getCurrentMeasurement());
}

void attemptToSendMeasurement(int16_t currentMeasurement)
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
