#include "PID.h"
#include "Control.h"
#include "Declare.h"
#include <Arduino.h>

 // Count number of sensor on the line
int count_on() {
  count1 = 0;
  for (int i = 0; i < 16; i++) {
    if (!digitalRead(input_pin[i])) {
      count1++;
    }
  }
  return count1;
}

// PID algorithm
void PID() {
  float Kp = 0.063;
  float Kd = 0.0001;
  error = sensor_position();
  float powerDifference = Kp * error + Kd * (error - lastError);
  lastError = error;
  powerDifference = constrain(powerDifference, -maxSpeed, maxSpeed);

  Serial.println("PID");
  if (powerDifference < 0) {
    set_motor(1, maxSpeed - powerDifference, 0, maxSpeed);
    //Serial.println("PID Trai");
  } else {
    set_motor(1, maxSpeed, 0, maxSpeed + powerDifference);
    //Serial.println("PID Phai");
  }
}

// Estimate the position of the sensor array
int sensor_position() {
  int input_array[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };
  for (int i = 0; i < 16; i++) {
    input_array[i] = 1 - digitalRead(input_pin[i]);
  }
  position_value = 0;
  int numer = 0;
  int denom = 0;
  for (int i = 0; i < 16; i++) {
    numer += input_array[i] * sensorweight[i]; //  Calculates the position of the sensor array over the line by weighted average method. The weights
    denom += input_array[i]; //  are assigned as per sensorWeight[]. A value of -7500 indicates that the line is over the leftmost
  } // sensor array
  //  Serial.print(numer);
  //  Serial.print("   ");
  //  Serial.println(denom);
  position_value = 750 - (numer / denom);
  return position_value;
}

