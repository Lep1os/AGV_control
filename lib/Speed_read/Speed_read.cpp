#include "Speed_read.h"
#include "Declare.h"
#include <Arduino.h>

void countLeftPulses() {
  pulseCountleft++;
};

void countRightPulses() {
  pulseCountright++;
};

// Calculate RPM every second
void print_RPM(unsigned long currentTime){
  if (currentTime - lastTime >= 1000) {
    noInterrupts(); // Disable interrupts temporarily to read pulseCount
    unsigned int count = pulseCountleft;
    unsigned int count = pulseCountright;
    pulseCountleft = 0; // Reset pulse count
    pulseCountright = 0;
    interrupts(); // Re-enable interrupts

    // Calculate RPM
    float LeftRPM = (count / (float)pulsesPerRevolution) * 60.0;
    float RightRPM = (count / (float)pulsesPerRevolution) * 60.0;
    
    // Print RPM values
    Serial.print("Left Motor RPM: ");
    Serial.println(LeftRPM);
    Serial.print("Right Motor RPM: ");
    Serial.println(RightRPM);
    
    // Update the lastTime
    lastTime = currentTime;
  }
}