/* Controller for the Teach-In Robot Model
 * so this is just a working control with the switches
 *
 * E7 and E8 control the arm
 * E5 and E6 control the base
 * E3 and E4 are for magnet on/off
 *
 * connect Magnet to M2, ARM to M3 and base to M4
 * 
 * 2019 - P. Gabriel - public domain
 */
#include <FTControl.h>

#define ARM 3
#define BASE 4
#define MAGNET 2
#define ARM_DOWN 7
#define ARM_UP 8
#define BASE_CW 6
#define BASE_CCW 5
#define MAGNET_ON 3
#define MAGNET_OFF 4
#define RECORD_START 1
#define RECORD_STOP 2

//the following values are for the potentiometers
// adjust them if you want to let the robot stop automatically

//#define BASE_MIN 680
//#define BASE_MAX 940
//#define ARM_MAX 1020
//#define ARM_MIN 345

#define BASE_MIN 0
#define BASE_MAX 1200
#define ARM_MAX 1200
#define ARM_MIN 0

int stateMagnet = 0;
int stateBase = 0;
int stateArm = 0;

FTControl ftc;

void debug(String s) {
  //  Serial.println(s);
}

void setup() {
  ftc.begin();
  //initialise motors
  for (int i=1; i<5; i++) {
    ftc.setSpeed(i,150);
    ftc.forward(i);
    ftc.stop(i);
  }
}

void loop() {
  if (ftc.checkE(ARM_DOWN) == LOW) {
    if (ftc.checkEY() > ARM_MIN) {
      debug("arm-forward");
      ftc.forward(ARM);
    } else {
      debug("arm-stop");
      ftc.stop(ARM);
    }  
  } else if (ftc.checkE(ARM_UP) == LOW) {
    if (ftc.checkEY() < ARM_MAX) {
      debug("arm-backward");
      ftc.backward(ARM);
    } else {
      debug("arm-stop");
      ftc.stop(ARM);
    }  
  } else {
    ftc.stop(ARM);
    debug("arm-off");
  }

  if (ftc.checkE(BASE_CW) == LOW) {
    if (ftc.checkEX() < BASE_MAX) {
      debug("base-forward");
      ftc.forward(BASE);
    } else {
      debug("base-stop");
      ftc.stop(BASE);
    }  
  } else if (ftc.checkE(BASE_CCW) == LOW) {
    if (ftc.checkEX() > BASE_MIN) {
      debug("base-backward");
      ftc.backward(BASE);
    } else {
      debug("base-stop");
      ftc.stop(BASE);
    }  
  } else {
    ftc.stop(BASE);
    debug("base-off");
  }

  if (ftc.checkE(MAGNET_ON) == LOW) {
     stateMagnet = 1;
     ftc.forward(2);
  } else if (ftc.checkE(MAGNET_OFF) == LOW) {
     stateMagnet = 0;
     ftc.stop(2);
  }
  debug("magnet-");
  if (stateMagnet == 1)
    debug("on");
  else
    debug("off");

  debug("base pos: " + ftc.checkEX());
  debug("arm  pos: " + ftc.checkEY());
  debug("");
  debug("");
  delay(2);
}
