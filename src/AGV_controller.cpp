#include <Arduino.h>
#include "Declare.h"
#include "Control.h"
#include "PID.h"
#include "Speed_read.h"



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }

  //Motor signals
  pinMode(FR1, OUTPUT);
  pinMode(FR2, OUTPUT);
  pinMode(SV1, OUTPUT);
  pinMode(SV2, OUTPUT);

  //Pins to read signal from esp
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
}
void check();
void loop() {
  unsigned long currentTime = millis();
  print_RPM(currentTime);
  check();
  //Serial.println(state);
  switch (state) {
  case 7:
    stopp();
    break;
  case 6:
    straight();
    break;
  case 5:
    back();
    break;
  case 4:
    left();
    break;
  case 3:
    right();
    break;
  case 0:
    follow_line();
    break;
  default:
    stopp();
    break;
  }
}

void check() {
  bitWrite(state, 0, digitalRead(3));
  bitWrite(state, 1, digitalRead(6));
  bitWrite(state, 2, digitalRead(7));
}











