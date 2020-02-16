/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED 7-Segment backpacks 
  ----> http://www.adafruit.com/products/881
  ----> http://www.adafruit.com/products/880
  ----> http://www.adafruit.com/products/879
  ----> http://www.adafruit.com/products/878

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistributionq
 ****************************************************/

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

int ToggleButton = 2;
int StartButton = 3;
int ToggleButtonRead = 0;
int StartButtonRead = 0;
volatile byte start = true;
volatile byte toggle = false;
int count = 0;


void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif
  matrix.begin(0x70);
  pinMode(ToggleButton, INPUT_PULLUP);
  pinMode(StartButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ToggleButton), ToggleHandler, RISING);
  attachInterrupt(digitalPinToInterrupt(StartButton), StartHandler, RISING);
 
}


// handles the increments
void ToggleHandler() {
  count += 300;
  toggle = true;
  start = false;
}

//starts and in the future might pause the timer if we so require
void StartHandler() {
  if (start == true){ start == false;
  }
  else {
  start = true;
  toggle = false;
  }
}


/*Important ::this code counts up to 100 minutes!!then goes into hexadecimal, this might need to be fixed
 * depending on what we want it to count up to. Should be a couple if statements.*/

void loop() {
  // try to print a number thats too long
  matrix.print(10000, DEC);
  matrix.writeDisplay();
  delay(500);

  // print the set time at the begining if the user has entered input
  if (toggle == true){
    boolean drawDots = false;
    uint16_t countin = count;
    matrix.writeDigitNum(0, (countin/600),drawDots );
    matrix.writeDigitNum(1, (countin/60)%10, drawDots);
    matrix.drawColon(drawDots);
    matrix.writeDigitNum(3, (countin/10)%6, drawDots);
    matrix.writeDigitNum(4, countin%10, drawDots);
     matrix.writeDisplay();
     delay(1000);
//  matrix.print(0xBEEF, HEX);
//  matrix.writeDisplay();
//  delay(500);
  }

//i
 else{ // print 00:00 time if the count is at 0
  if (count == 0){
    boolean drawDots = false;
    uint16_t countin = count;
    matrix.writeDigitNum(0, (countin/600),drawDots );
    matrix.writeDigitNum(1, (countin/60)%10, drawDots);
    matrix.drawColon(drawDots);
    matrix.writeDigitNum(3, (countin/10)%6, drawDots);
    matrix.writeDigitNum(4, countin%10, drawDots);
    matrix.writeDisplay();
    delay(1000);
    }
//  matrix.print(00.00);
//  matrix.writeDisplay();
//  delay(1000);
 
  //count down the time
    uint16_t blinkcounter = 0;
    boolean drawDots = false;
    for (uint16_t counter = count; counter > 0; counter --) {
      if (toggle == true){
        return;
      }
      matrix.writeDigitNum(0, (counter / 600), drawDots);
      matrix.writeDigitNum(1, (counter / 60) % 10, drawDots);
      matrix.drawColon(drawDots);
      matrix.writeDigitNum(3, (counter / 10) % 6, drawDots);
      matrix.writeDigitNum(4, counter % 10, drawDots);

     
      blinkcounter+=50;
      if (blinkcounter < 500) {
        drawDots = false;
      } else if (blinkcounter < 1000) {
        drawDots = true;
      } else {
        blinkcounter = 0;
      }
      matrix.writeDisplay();
      delay(1000);
  }
  }
}
