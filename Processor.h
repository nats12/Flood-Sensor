/*
  Processor.h - Library for dealing with processor settings i.e. its interrupt/led pins.
  Created by Natalie Mclaren, December 18, 2017.
*/

#ifndef Processor_h
#define Processor_h
//#include <TheThingsNetwork.h>
#include "SDCard.h"
#include "Sensor.h"

/**
 * 
 */
class Processor
{  
  public:
    volatile byte state;
    int delayPeriod;

    // Constructors
    Processor(Sensor *sensor, SDCard *sdCard, byte ledPin, byte interruptPin); //TheThingsNetwork *ttn, byte ledPin, byte interruptPin);

    // Main States
    void init();
    void readingProcess();

    // AR mode
    void activateOrDeactivateARMode();
    void adjustIgnoreThreshold(int newIgnoreThreshold);
    void adjustARModeDelay(int newDelayPeriod);
    void adjustARModeThreshold(int newActivationThreshold);

    // Helpers
    void writeStatus();
    void delayWithPeriod();
    void changeMeasurementPeriod(String minutes);
    float getBatteryVoltage();
    void printToSDLog(int lastMeasurementSent);
    void printCurrentMeasurementToSD(int currentMeasurement);
    
    // Setters
    void setSpreadingFactor(int spreadFactor);
    void setAppEui(char *appEui);
    void setAppKey(char *appKey);

  private:
    // Components
    Sensor *sensor;
    SDCard *sdCard;
//    TheThingsNetwork *ttn;
    
    // Initial depth
    int initialRiverDepth;
    // AR mode variables
    int delayPeriodARMode;
    int ARModeActivationThreshold; 
    int ignoreThreshold;
    // LoRaWAN keys + spread factor
    int spreadFactor = 7;
    char *appEui;
    char *appKey;
    // Pins
    int ledPin;
    int interruptPin;
};

#endif
