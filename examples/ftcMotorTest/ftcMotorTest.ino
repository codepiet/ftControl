/* Test program for testing the motors
 * The program will cycle through the motors 
 * and run the motors forward then backward.
 * 2019 - P. Gabriel - public domain
 */
#include <FTControl.h>

FTControl ftc;

void debug(String s) {
  Serial.println(s);
}

void setup() {
  Serial.begin(9600);
  ftc.begin();
}

void loop() {
 String s;
 for (int i=1; i<5; i++) {
    s = "Motor ";
    s = s + i;
    s = s + ": Forward";
    debug(s);
    ftc.setSpeed(i,180);
    ftc.forward(i);
    delay(1000);
    ftc.stop(i);
    delay(1000);
    s = "Motor ";
    s = s + i;
    s = s + ": Backward";
    debug(s);
    ftc.backward(i);
    delay(1000);
    ftc.stop(i);
    delay(1000);
  }
}
