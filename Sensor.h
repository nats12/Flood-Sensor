/*
  Sensor.h - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/

#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"

/**
 * 
 */
class Sensor
{
  public:
    int16_t currentRiverLevel;
    int16_t distanceToRiverBed;
    int16_t rangeDifferenceThreshold = 50;
    int16_t lastMeasurementSent;
    byte sensorPowerPin;
    byte sensorAnalogPin;
  
    Sensor(byte sensorAnalogPin, byte sensorPowerPin);
    void init();
    int16_t getCurrentMeasurement();
    bool isCurrentWorthSending(int16_t currentMeasurement);
  private:

};

#endif
