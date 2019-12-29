/* Controller for the Teach-In Robot Model
 * with record and playback function
 * 2019 - P. Gabriel - public domain
 *
 * E7 and E8 control the arm
 * E5 and E6 control the base
 * E3 and E4 are for magnet on/off
 * E1 controls recording and stop
 * E2 controls data point storage and playback
 *
 * connect Magnet to M2, ARM to M3 and base to M4
 *
 * When in IDLE mode you can record datapoints:
 *   1. press E1 - then you are in record mode
 *   2. you can move the robot
 *   3. store position with E2
 *   4. stop the recording with E1
 * Then you can playback with E2
 * 
 * Additionally you'll need a 20x4 display connected via I2C.
 * If you do not have it - please comment all lines with lcd
 */
#include <FTControl.h>
#include <LiquidCrystal_I2C.h>

#define ARM 3
#define BASE 4
#define MAGNET 2
#define ARM_DOWN 7
#define ARM_UP 8
#define BASE_CW 6
#define BASE_CCW 5
#define MAGNET_ON 3
#define MAGNET_OFF 4
#define MODE 1
#define SELECT 2

#define MODE_IDLE 100
#define MODE_REC 101
#define MODE_PLAY 102

//the following values are for the potentiometers
// adjust them if you want to let the robot stop automatically
#define BASE_MIN 680
#define BASE_MAX 950
#define ARM_MAX 850
#define ARM_MIN 150

#define ARM_SPEED 150
#define BASE_SPEED 230
#define MAGNET_POWER 255

#define DATA_MAX 100

struct rdata {
  boolean magnet;
  int base;
  int arm;
};

// data array to hold robot states
rdata data[DATA_MAX];

int stateMagnet = 0;
int opMode = MODE_IDLE;
int dataPosition = 0;

FTControl ftc;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Teach-In FTC");

  ftc.begin();
  //initialise motors
  ftc.setSpeed(ARM, ARM_SPEED);
  ftc.forward(ARM);
  ftc.stop(ARM);
  ftc.setSpeed(BASE, BASE_SPEED);
  ftc.forward(BASE);
  ftc.stop(BASE);
  ftc.setSpeed(MAGNET, MAGNET_POWER);
  ftc.forward(MAGNET);
  ftc.stop(MAGNET);
}

void printSerial(rdata d) {
  Serial.print("Position: ");
  Serial.println(dataPosition);
  Serial.print("  ARM:    ");  Serial.print(d.arm);
  Serial.print("  BASE:   ");  Serial.print(d.base);
  Serial.print("  MAGNET: ");  Serial.println(d.magnet);
}

void printToLCD() {
  String s = "";
  lcd.setCursor(0, 1);
  lcd.print("E: ");
  for (int i = 1; i < 9; i++) {
    s += 1-ftc.checkE(i);
  }
  lcd.print(s);
  lcd.setCursor(0, 2);
  lcd.print("EX:");  lcd.print(ftc.checkEX()); lcd.print("  ");
  lcd.setCursor(0, 3);
  lcd.print("EY:");  lcd.print(ftc.checkEY()); lcd.print("  ");
  //update LCD
  lcd.setCursor(8, 3);
  if (opMode == MODE_IDLE) lcd.print("IDLE");
  else if (opMode == MODE_PLAY) lcd.print("PLAY");
  else if (opMode == MODE_REC) lcd.print("REC ");
  lcd.setCursor(13, 0);
  lcd.print(" P:");
  lcd.print(dataPosition); lcd.print("  ");
  lcd.setCursor(14, 1);
  lcd.print("B:");
  lcd.print(data[dataPosition].base); lcd.print("  ");
  lcd.setCursor(14, 2);
  lcd.print("A:");
  lcd.print(data[dataPosition].arm); lcd.print("  ");
  lcd.setCursor(14, 3);
  lcd.print("M:");
  lcd.print(data[dataPosition].magnet ? 1 : 0);
}

void loop() {
  if (millis() % 300 < 6) {
    printToLCD();
  }
  //playback record control
  if (opMode == MODE_IDLE && ftc.checkE(MODE) == LOW) {
    opMode = MODE_REC;
    for (int i = 0; i < DATA_MAX; i++) {
      data[i] = {false, -1, -1};
    }
    dataPosition = 0;
    delay(300);
  } else if (opMode == MODE_REC && ftc.checkE(MODE) == LOW) {
    opMode = MODE_IDLE;
    delay(300);
  } else if (opMode == MODE_IDLE && ftc.checkE(SELECT) == LOW) {
    opMode = MODE_PLAY;
    dataPosition = 0;
    delay(300);
  } else if (opMode == MODE_PLAY && ftc.checkE(SELECT) == LOW) {
    opMode = MODE_IDLE;
    delay(300);
  } else if (opMode == MODE_REC && ftc.checkE(SELECT) == LOW) {
    if (dataPosition < 100) {
      //store current position in array
      data[dataPosition] = { stateMagnet == 1, ftc.checkEX(), ftc.checkEY() };
      dataPosition++;
    } else {
      opMode = MODE_IDLE;
    }
    delay(300);
  }
  if (opMode == MODE_PLAY) {
    rdata next = data[dataPosition];
    printSerial(next);
    if (next.arm == -1) {
      dataPosition = 0;
      opMode = MODE_IDLE;
    } else {
      //check magnet
      next.magnet ? ftc.forward(MAGNET) : ftc.stop(MAGNET);
      //goto position
      if (ftc.checkEY() < next.arm) {
        ftc.backward(ARM);
        while (ftc.checkEY() < next.arm && ftc.checkEY() < ARM_MAX) ;
        ftc.stop(ARM);
      } else if (ftc.checkEY() > next.arm) {
        ftc.forward(ARM);
        while (ftc.checkEY() > next.arm && ftc.checkEY() > ARM_MIN) ;
        ftc.stop(ARM);
      }

      if (ftc.checkEX() < next.base) {
        ftc.forward(BASE);
        while (ftc.checkEX() < next.base) ;
        ftc.stop(BASE);
      } else {
        ftc.backward(BASE);
        while (ftc.checkEX() > next.base) ;
        ftc.stop(BASE);
      }
      dataPosition++;
    }
  }
  if (opMode == MODE_IDLE || opMode == MODE_REC) {
    if (ftc.checkE(ARM_DOWN) == LOW) {
      if (ftc.checkEY() > ARM_MIN) {
        ftc.forward(ARM);
      } else {
        ftc.stop(ARM);
      }
    } else if (ftc.checkE(ARM_UP) == LOW) {
      if (ftc.checkEY() < ARM_MAX) {
        ftc.backward(ARM);
      } else {
        ftc.stop(ARM);
      }
    } else {
      ftc.stop(ARM);
    }

    if (ftc.checkE(BASE_CW) == LOW) {
      if (ftc.checkEX() < BASE_MAX) {
        ftc.forward(BASE);
      } else {
        ftc.stop(BASE);
      }
    } else if (ftc.checkE(BASE_CCW) == LOW) {
      if (ftc.checkEX() > BASE_MIN) {
        ftc.backward(BASE);
      } else {
        ftc.stop(BASE);
      }
    } else {
      ftc.stop(BASE);
    }

    if (ftc.checkE(MAGNET_ON) == LOW) {
      stateMagnet = 1;
      ftc.forward(MAGNET);
    } else if (ftc.checkE(MAGNET_OFF) == LOW) {
      stateMagnet = 0;
      ftc.stop(MAGNET);
    }
  }
  delay(2);
}
