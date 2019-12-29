/*
  FTControl.cpp - V1.1 - Library for controlling legacy Fischer Technik computing.
  Created by Peter Gabriel, December, 2019.
  Released into the public domain.

  History: v1.1: changed E1 and E2 to pins 11 resp. 12 due to I2C conflicts
*/

#include "Arduino.h"
#include "FTControl.h"

#define bE1 11
#define bE2 12
#define bE3 4
#define bE4 5
#define bE5 6
#define bE6 7
#define bE7 8
#define bE8 9

#define pEX A0
#define pEY A1

FTControl::FTControl()
{
  pinMode(pEX, INPUT);
  pinMode(pEY, INPUT);
  pinMode(bE1, INPUT_PULLUP);
  pinMode(bE2, INPUT_PULLUP);
  pinMode(bE3, INPUT_PULLUP);
  pinMode(bE4, INPUT_PULLUP);
  pinMode(bE5, INPUT_PULLUP);
  pinMode(bE6, INPUT_PULLUP);
  pinMode(bE7, INPUT_PULLUP);
  pinMode(bE8, INPUT_PULLUP);

  _AFMS = Adafruit_MotorShield();
}

void FTControl::begin() {
  _AFMS.begin();
}

void FTControl::forward(int nr) {
   Adafruit_DCMotor *_m = _AFMS.getMotor(nr);
   _m->run(FORWARD);
}

void FTControl::backward(int nr) {
   Adafruit_DCMotor *_m = _AFMS.getMotor(nr);
   _m->run(BACKWARD);
}

void FTControl::stop(int nr) {
   Adafruit_DCMotor *_m = _AFMS.getMotor(nr);
   _m->run(RELEASE);
}

void FTControl::setSpeed(int nr, int speed) {
   Adafruit_DCMotor *_m = _AFMS.getMotor(nr);
   _m->setSpeed(speed);
}

int FTControl::checkEX()
{
  return analogRead(pEX);
}

int FTControl::checkEY()
{
  return analogRead(pEY);
}

uint8_t FTControl::checkE(int num)
{
  int pin = bE1;
  if (num == 8)
    pin = bE8;
  else if (num == 8)
    pin = bE8;
  else if (num == 7)
    pin = bE7;
  else if (num == 6)
    pin = bE6;
  else if (num == 5)
    pin = bE5;
  else if (num == 4)
    pin = bE4;
  else if (num == 3)
    pin = bE3;
  else if (num == 2)
    pin = bE2;
  return digitalRead(pin);
}
