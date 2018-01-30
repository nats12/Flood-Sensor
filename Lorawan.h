/*
  Lorawan.h - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/

#ifndef Lorawan_h
#define Lorawan_h
#define freqPlan TTN_FP_EU868

#include <TheThingsNetwork.h>


/**
 * 
 */
class Lorawan
{
  public:
    Lorawan(uint8_t spreadFactor);

    bool join();
    ttn_response_t sendReading(int16_t reading, uint8_t powerLevel);
    ttn_response_t sendStillAlive(uint8_t powerLevel);
    ttn_response_t sendGenericError(uint8_t powerLevel);
    ttn_response_t sendMicrocontrollerError(uint8_t powerLevel);
    ttn_response_t sendSensorError(uint8_t powerLevel);
    ttn_response_t sendBatteryError(uint8_t powerLevel);
    ttn_response_t sendStorageError(uint8_t powerLevel);
    uint8_t getSpreadFactor();
    char* getAppEui();
    char* getAppKey();
    void setSpreadFactor(uint8_t spreadFactor);
    void setAppEui(char *appEui);
    void setAppKey(char *appKey);
    
  private:
    TheThingsNetwork ttn;
    uint8_t spreadFactor;
    char *appEui = "70B3D57EF00000C3";
    char *appKey = "9219CFB30320D36373218DB4EF00D1CB";
};

#endif
