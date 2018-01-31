/*
  Sensor.h - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/

#ifndef Sensor_h
#define Sensor_h

/**
 * 
 */
class Sensor
{
  public:
    int currentRiverLevel;
    int distanceToRiverBed;
    int rangeDifferenceThreshold;
    int lastMeasurementSent;
    int analogPin;
  
    Sensor(int analogPin);
    int getCurrentMeasurement();
    bool isCurrentWorthSending(int currentMeasurement);
  private:

};

#endif
