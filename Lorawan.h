/*
  Lorawan.h - Library for ultrasonic sensor water level measurements.
  Created by Natalie Mclaren, December 17, 2017.
*/

#ifndef Lorawan_h
#define Lorawan_h

#include <TheThingsNetwork.h>

/**
 * 
 */
class Lorawan: public TheThingsNetwork
{
  public:
    Lorawan(Stream &modemStream, Stream &debugStream, ttn_fp_t fp, uint8_t sf = TTN_DEFAULT_SF, uint8_t fsb = TTN_DEFAULT_FSB);

    bool join();
    bool provision();
    ttn_response_t sendReading(int16_t reading, uint8_t powerLevel);
    ttn_response_t sendStillAlive(uint8_t powerLevel);
    ttn_response_t sendGenericError(uint8_t powerLevel);
    ttn_response_t sendMicrocontrollerError(uint8_t powerLevel);
    ttn_response_t sendSensorError(uint8_t powerLevel);
    ttn_response_t sendBatteryError(uint8_t powerLevel);
    ttn_response_t sendStorageError(uint8_t powerLevel);
    uint8_t getSpreadFactor();
    char* getCharAppEui();
    char* getAppKey();
    void setSpreadFactor(uint8_t spreadFactor);
    void setCharAppEui(char *appEui);
    void setAppKey(char *appKey);
    
  private:
    char *appEui = "70B3D57EF00000C3";
    char *appKey = "76B462F516EADD80FE5153D525BC5B67";
};

#endif
