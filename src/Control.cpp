#include "Control.h"
#include "PID.h"
#include "Declare.h"
#include <Arduino.h>

//Following line control
void follow_line() {
  int input_value = data_value();
  switch (count_on()) {
  case 0:
    stopp();
    break;
  case 1:
    PID();
    break;
  case 2:
    if (input_value % 3 == 0)
      PID();
    break;
  case 3:
    if (input_value % 7 == 0)
      PID();
    break;

  case 4:
    if (input_value % 15 == 0)
      PID();
    else if ((input_value == 0b1100000110000000) || (input_value == 0b1000001110000000) || (input_value == 0b1000000111000000))
      rotate(LEFT);
    else if ((input_value == 0b0000000110000011) || (input_value == 0b0000001110000001) || (input_value == 0b0000000111000001))
      rotate(RIGHT);
    break;

  case 5:
    if (input_value % 31 == 0)
      PID();
    else if ((input_value == 0b1100001110000000) || (input_value == 0b1100000111000000) || (input_value == 0b1000001111000000))
      rotate(LEFT);
    else if ((input_value == 0b0000001110000011) || (input_value == 0b0000000111000011) || (input_value == 0b0000001111000001))
      rotate(RIGHT);
    break;

  case 6:
  case 7:
    if ((input_value & 0b0000000000111111) == 0)
      rotate(LEFT);
    else if ((input_value & 0b1111110000000000) == 0)
      rotate(RIGHT);
    break;

  case 8:
  case 9:
    if ((input_value & 0b0000000000011111) == 0)
      rotate(LEFT);
    else if ((input_value & 0b1111100000000000) == 0)
      rotate(RIGHT);
    break;

  case 10:
  case 11:
    if ((input_value & 0b0000000000001111) == 0)
      rotate(LEFT);
    else if ((input_value & 0b1111000000000000) == 0)
      rotate(RIGHT);
    break;

  case 12:
  case 13:
    if ((input_value & 0b0000000000000111) == 0)
      rotate(LEFT);
    else if ((input_value & 0b1110000000000000) == 0)
      rotate(RIGHT);
    break;

  case 14:
    if ((input_value & 0b0000000000000011) == 0)
      rotate(LEFT);
    else if ((input_value & 0b1100000000000000) == 0)
      rotate(RIGHT);
    break;

  case 15:
  case 16:
    rotate(LEFT);
    lastError = 0;
    break;
  }
}


//Fundamental movements control
//Function that make the robot go forward
void straight() {
  Serial.println("straight");
  digitalWrite(FR1, 1);
  analogWrite(SV1, 100);
  digitalWrite(FR2, 0);
  analogWrite(SV2, 100);
}
//Function that make the robot go backward
void back() {
  Serial.println("back");
  digitalWrite(FR1, 0);
  analogWrite(SV1, 100);
  digitalWrite(FR2, 1);
  analogWrite(SV2, 100);
}
//Function that make the robot turn right
void right() {
  Serial.println("right");
  digitalWrite(FR1, 0);
  analogWrite(SV1, 70);
  digitalWrite(FR2, 0);
  analogWrite(SV2, 70);
}
//Fucntion that make the robot turn left
void left() {
  Serial.println("left");
  digitalWrite(FR1, 1);
  analogWrite(SV1, 70);
  digitalWrite(FR2, 1);
  analogWrite(SV2, 70);
}
//Function that make the robot stop
void stopp() {
  Serial.println("stop");
  digitalWrite(FR1, 0);
  analogWrite(SV1, 0);
  digitalWrite(FR2, 0);
  analogWrite(SV2, 0);
}
//Set up function for motor speed
void set_motor(int direction_left, int speed_left, int direction_right, int speed_right) {
  digitalWrite(FR1, direction_left);
  analogWrite(SV1, speed_left);
  digitalWrite(FR2, direction_right);
  analogWrite(SV2, speed_right);
}
void rotate(int side) {
  straight();
  delay(2000);
  stopp();
  while (data_value() != 0b0000001111000000) { //May have to check middle 4 sensors if there is loop in track

    //Perform rotation
    if (side == RIGHT) {
      set_motor(0, rotateSpeed, 0, rotateSpeed);
      Serial.println("Trai");
    } else if (side == LEFT) {
      set_motor(1, rotateSpeed, 1, rotateSpeed);
      Serial.println("Phai");
    }
  }
}
unsigned int data_value() {
  unsigned int input_value = 0b0000000000000000;
  for (int i = 0; i < 16; i++) {
    bitWrite(input_value, i, 1 - digitalRead(input_pin[i]));
  }
  return input_value;
}