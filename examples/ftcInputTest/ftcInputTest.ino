/* Test program for showing and testing the inputs
 * when showing 1 the switch is not pressed, 0 is pressed
 * 
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
  for (int i=1; i<9; i++) {
    s = "Switch ";
    s = s + i;
    s = s + ": ";
    s = s + ftc.checkE(i);
    debug(s);
  }
  s = "Poti X:   ";
  s = s + ftc.checkEX();
  debug(s);
  
  s = "Poti Y:   ";
  s = s + ftc.checkEY();
  debug(s);

  delay(1000);
}
