/**
  Processor.h - Library for dealing with processor settings i.e. its engineeringMenuJumper/led pins.
  Created by Natalie Mclaren, December 18, 2017.
*/

#ifndef Processor_h
#define Processor_h

#include "SDCard.h"
#include "Sensor.h"
#include "Lorawan.h"

/**
 * 
 */
class Processor
{  
  public:
    int16_t delayPeriod;
    int16_t measurementPeriod;
    bool ARModeOn;

    // Constructors
    Processor(Sensor *sensor, SDCard *sdCard, Lorawan *lorawan, byte interrputPin, int16_t delayPeriod = 300000, int16_t delayPeriodARMode = 600000, int16_t ARModeActivationThreshold = 200000, int16_t ignoreThreshold = 0); 

    // Main States
    void init();
    void readingProcess();

    // AR mode
    void activateOrDeactivateARMode();
    void adjustIgnoreThreshold(int16_t newIgnoreThreshold);
    void adjustARModeDelay(int16_t newDelayPeriod);
    void adjustARModeThreshold(int16_t newActivationThreshold);

    // Helpers
    void delayWithPeriod();
    void changeMeasurementPeriod(int16_t minutes);
    void recalibrateSensor();
    void triggerClearFlash();
    void adjustAppEui(String newAppEui);
    void adjustAppKey(String newAppKey);
    float getBatteryVoltage();
    uint8_t getBatteryVoltageByte();
    uint8_t getEstimatedPowerLevel();
    void printMeasurementToSDLog(int16_t measurement);
    
  private:
    // Components
    Sensor *sensor;
    SDCard *sdCard;
    Lorawan *lorawan;

    // Still Here count
    uint8_t stillHereCount;
    // Initial depth
    int16_t initialRiverDepth;
    // AR mode variables
    int16_t delayPeriodARMode;
    int16_t ARModeActivationThreshold; 
    int16_t ignoreThreshold;
    // Pins
    byte engineeringMenuJumperPin;
};

#endif
