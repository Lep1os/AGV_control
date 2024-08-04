#include <Arduino.h>
int FR1 = 44;
int FR2 = 45;
int SV1 = 10;
int SV2 = 11;

int state;
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
}; // all the input pin connect to the sensor
int input_array[16];
uint16_t input_value = 0b0000000000000000; // store the input value                                                            
int count1 = 0;
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
int position_value = 0;
int error, lastError;
int maxSpeed = 50;
int rotateSpeed = 25;
#define LEFT 69
#define RIGHT 420

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }
  pinMode(FR1, OUTPUT);
  pinMode(FR2, OUTPUT);
  pinMode(SV1, OUTPUT);
  pinMode(SV2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
}

void loop() {
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
    /*Code to turn left at a T-section
                 if((totalDistance > 750) && (leftTurn < 5))
                 {
                   do
                   { 
                     QTRX.getSensorsNormalized(sensorValue, TOGGLE);
                     input_value = QTRX.getSensorsBinary(sensorValue);
                     // Skip turn if distance covered while within loop is > 30cm
                     if((totalDistance - loopStartDistance) > 30)
                     {
                       digitalWriteFast(side, LOW);
                       return;
                     }
                   }while(input_value & 0b0000000110000000);
                 */
    rotate(LEFT);
    lastError = 0;
    break;
  }
}

unsigned int data_value() {
  unsigned int input_value = 0b0000000000000000;
  for (int i = 0; i < 16; i++) {
    bitWrite(input_value, i, 1 - digitalRead(input_pin[i]));
  }
  return input_value;
}

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

void PID() {
  float Kp = 0.063;
  float Kd = 0.0001;
  error = sensor_position();
  float powerDifference = Kp * error + Kd * (error - lastError);
  lastError = error;
  powerDifference = constrain(powerDifference, -maxSpeed, maxSpeed);
  //Serial.print(powerDifference);
  //Serial.print("   ");
  Serial.println("PID");
  if (powerDifference < 0) {
    set_motor(1, maxSpeed - powerDifference, 0, maxSpeed);
    //Serial.println("PID Trai");
  } else {
    set_motor(1, maxSpeed, 0, maxSpeed + powerDifference);
    //Serial.println("PID Phai");
  }
}

int count_on() {
  count1 = 0;
  for (int i = 0; i < 16; i++) {
    if (!digitalRead(input_pin[i])) { // count number of sensor on the line
      count1++;
    }
  }
  return count1;
}

void straight() {
  Serial.println("straight");
  digitalWrite(FR1, 1);
  analogWrite(SV1, 100);
  digitalWrite(FR2, 0);
  analogWrite(SV2, 100);
}

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

//Function that make the robot stopp
void stopp() {
  Serial.println("stop");
  digitalWrite(FR1, 0);
  analogWrite(SV1, 0);
  digitalWrite(FR2, 0);
  analogWrite(SV2, 0);
}

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
