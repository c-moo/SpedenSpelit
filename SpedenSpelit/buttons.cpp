#include "buttons.h"

volatile unsigned long lastMillis = 0;    // debouncing variable used in buttons.cpp

void initButtonsAndButtonInterrupts(void) {
  PCICR |= 0b00000100;   // Using the bitwise operator modifies only the 2 bit to be 1 = enabling pinChangeInterrupts for port D.
  PCMSK2 |= 0b01111100;  // Turn on pins PD2-PD6

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
 
}

ISR(PCINT2_vect)  // Port D, PCINT16 - PCINT23
{
  if ((millis() - lastMillis) > 200) {
    if ((PIND & bit(2)) == 0) {
      buttonNumber = 0;
    } 
    else if ((PIND & bit(3)) == 0) {
      buttonNumber = 1;
    } 
    else if ((PIND & bit(4)) == 0) {
      buttonNumber = 2;
    } 
    else if ((PIND & bit(5)) == 0) {
      buttonNumber = 3;
    } 
    else if ((PIND & bit(6)) == 0) {
      buttonNumber = 4;
    }
  }
lastMillis = millis();
}