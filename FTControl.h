/*
  FTControl.h - Library for controlling 
  legacy FischerTechnik computing stuff with an arduino
  and an adafruit motor shield v2
  Created by Peter Gabriel, December 2019
  Released into the public domain.
*/
#ifndef FTControl_h
#define FTControl_h

#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>

class FTControl
{
  public:
    FTControl();
    void begin();
    uint8_t checkE(int num);
    int checkEX();
    int checkEY();
    void forward(int nr);
    void backward(int nr);
    void stop(int nr);
    void setSpeed(int nr, int speed);
  private:
    Adafruit_MotorShield _AFMS; 
};

#endif
