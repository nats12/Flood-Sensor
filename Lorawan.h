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
    Lorawan(int spreadFactor);
    
    int getSpreadFactor();
    void setSpreadFactor(int spreadFactor);
  private:
    TheThingsNetwork ttn(Serial1, Serial, freqPlan, sFactor);
    int spreadFactor;
    

};

#endif
