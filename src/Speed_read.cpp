#include "Speed_read.h"
#include "Declare.h"
#include <Arduino.h>

void setup(){
pinMode(speedPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(speedPin), countPulses, RISING);
}
void countPulses() {
  pulseCount++;
};

// Calculate RPM every second
void print_RPM(unsigned long currentTime){
  if (currentTime - lastTime >= 1000) {
    noInterrupts(); // Disable interrupts temporarily to read pulseCount
    unsigned int count = pulseCount;
    pulseCount = 0; // Reset pulse count
    interrupts(); // Re-enable interrupts

    // Calculate RPM
    float rpm = (count / (float)pulsesPerRevolution) * 60.0;
    
    // Print RPM value
    Serial.print("RPM: ");
    Serial.println(rpm);
    
    // Update the lastTime
    lastTime = currentTime;
  }
}