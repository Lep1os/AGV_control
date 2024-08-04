#include <Arduino.h>
#include "Declare.h"
#include "Control.h"
#include "PID.h"
#include "Speed_read.h"



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //Line sensor bits input
  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }

  // Speed signal inputs
  pinMode(speedPinleft, INPUT);
  pinMode(speedPinright, INPUT);

  // Attach interrupts for both motors
  attachInterrupt(digitalPinToInterrupt(speedPinleft), countLeftPulses, RISING);
  attachInterrupt(digitalPinToInterrupt(speedPinright), countRightPulses, RISING);

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


void loop() {
  unsigned long currentTime = millis();
  print_RPM(currentTime);
  check();
  switch (state) {
  case 7: // 0b111
    stopp();
    break;
  case 6: // 0b110
    straight();
    break;
  case 5:// 0b101
    back();
    break;
  case 4:// 0b100
    left();
    break;
  case 3:// 0b011
    right();
    break;
  case 0://0b000
    follow_line();
    break;
  default:
    stopp();
    break;
  }
}

// Check the command code sent by the esp
void check() {
  bitWrite(state, 0, digitalRead(3));
  bitWrite(state, 1, digitalRead(6));
  bitWrite(state, 2, digitalRead(7));
}











