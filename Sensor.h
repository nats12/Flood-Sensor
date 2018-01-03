/*
  Sensor.h - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/


#ifndef Sensor_h
#define Sensor_h
#include "SDCard.h"
#include "Arduino.h"


/**
 * 
 */
class Sensor
{ 
  
  SDCard *sdCard;
  
  public:
    Sensor(SDCard *sdCard);
    int getCurrentMeasurement();
    bool isCurrentWorthSending(int currentMeasurement);
    bool sendMeasurement(int currentMeasurement);
    void attemptToSendMeasurement(int currentMeasurement);
    void startReadingProcess();
    void changeMeasurementPeriod(String minutes);

    int currentRiverLevel;
    int distanceToRiverBed;
    int rangeDifferenceThreshold;
    int lastMeasurementSent;
    int analogPin;
   
  private:

};

#endif
