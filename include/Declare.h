#pragma once
//Declare the output speed ports for motors
//Left
#define FR1  44
//Right
#define FR2  45

//Declare the rotating direction ports for motors
//Left 
#define SV1  10
//Right
#define SV2  11

//Store the commnad from esp using
int state;

//Declare the array of 16-bit line sensor input pins
int input_pin[] = {
  A15,
  A14,
  A13,
  A12,
  A11,
  A10,
  A9,
  A8,
  A7,
  A6,
  A5,
  A4,
  A3,
  4,
  5,
  2
}; 

//Store input value
int input_array[16];
uint16_t input_value = 0b0000000000000000; 

//Store the detected points from line sensor
int count1 = 0;

//The weight value equally distributed along 16 detecting points of line sensor from right to left
const int sensorweight[] = {
  0,
  100,
  200,
  300,
  400,
  500,
  600,
  700,
  800,
  900,
  1000,
  1100,
  1200,
  1300,
  1400,
  1500
};

//Declare variables for PID
int position_value = 0;
int error, lastError;
int maxSpeed = 50;
int rotateSpeed = 25;
#define LEFT 69
#define RIGHT 420

//Declare variables for speed reading
const int speedPin = 12; 
volatile unsigned int pulseCount = 0;
unsigned long lastTime = 0;
unsigned int pulsesPerRevolution = 6;