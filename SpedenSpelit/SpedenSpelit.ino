#define MusicOn true

#include "display.h"
#include "buttons.h"
#include "leds.h"
#include "SpedenSpelit.h"

// 2 addiotional libraries
#include "DFRobotDFPlayerMini.h" // audio player
#include <avr/wdt.h> // arduinos own library that we use for the reboot

DFRobotDFPlayerMini myDFPlayer; 

volatile int buttonNumber = -1;           // for buttons interrupt handler
volatile bool newTimerInterrupt = false; // number generator and leds
volatile bool gameStarted = false;       // variable to communicate if game started
volatile bool speedUpTimerVar = false;
volatile int timerSpeed = 0;

const int arraySize = 100;
int timers[9] = { 14203, 12912, 11738, 10671, 9701, 8819, 8017, 7288, 6626 }; // array where we have the 10% speed ups for every 10th buttonpresses
int randomNumbers[arraySize];
int userNumbers[arraySize];
int randomNumbersGenerated;
int buttonsPushed;
int newNumber;
int oldNumber;

void setup()
{
  // Setup for DFPlayer mini our audio module using rx and tx pins 0-1 for communication
  Serial.begin(9600);
  myDFPlayer.begin(Serial); // using normal serial rx and tx 0-1 pins
  myDFPlayer.volume(15);  //Set volume value. From 0 to 30
  myDFPlayer.enableLoop(); // enables loop for the song


  //  Initializing all modules from .h files
  initializeDisplay(); 
  initializeLeds(); 
  initButtonsAndButtonInterrupts();
  initializeTimer();

  // -----------------------------------------------------------------------------------------------------------------
  showResult(0); 
}

void loop()
{
  if(buttonNumber>-1) {
    if (buttonNumber == 4) {
      if (!gameStarted) {
      startTheGame();
      buttonNumber = -1;
      }
    }
    else if (gameStarted) {
      checkGame(buttonNumber);
      buttonNumber = -1;
    }
  }

  if(newTimerInterrupt == true) {
    oldNumber = newNumber;
    while(newNumber == oldNumber) // loops until we get a different number than last
    {
      newNumber = random(0,4);
    }
    
    setLed(newNumber);
    randomNumbers[randomNumbersGenerated] = newNumber;
    randomNumbersGenerated += 1;

    newTimerInterrupt = false;
    if (randomNumbersGenerated > 99) { // stop timer if 100 numbers have been generated
      TCCR1B &= (1 <<WGM12);
    }
  }
}

void initializeTimer(void)
{
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 15624;            // compare match register 16MHz/1024/1Hz
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10); // 1024 prescaler 
  interrupts(); 
}

ISR(TIMER1_COMPA_vect)
{
  newTimerInterrupt = true;
  if (speedUpTimerVar) {  // moved timer speed up to interruption service so game doesn't freeze
    OCR1A = (timers[timerSpeed]);    // speeding up the timer using array with pre calculated OCR1A values
    speedUpTimerVar = false;       
    timerSpeed += 1;                 // [16 000 000 / (1024 * OCR1A) + 1] = HZ ...... Arduino clock speed / (prescaler * compare match + 1) = Hz
  }
}

void checkGame(byte nbrOfButtonPush)
{
  userNumbers[buttonsPushed] = nbrOfButtonPush;

  if ( nbrOfButtonPush == randomNumbers[buttonsPushed] ) { 

    buttonsPushed += 1;
    showResult(buttonsPushed);
    if ((buttonsPushed % 10) == 0) { 
      speedUpTimerVar  = true;
      if (buttonsPushed > 99) {  // WIN = does lightshow and plays victory sound
        show2(3);      
        myDFPlayer.disableLoop();
        delay(500);
        myDFPlayer.play(4);
        delay(1500);
        reboot();
      }
    }
  }
  else {
    myDFPlayer.disableLoop(); 
    myDFPlayer.play(1); // plays defeat sound
    gameStarted = false;
    TCCR1B &= (1 <<WGM12); // stop timer 
      setAllLeds();
      delay(500);
      clearAllLeds();
      delay(500);
      setAllLeds();
      delay(500);
      clearAllLeds();
      delay(500);
    reboot();
  }
}


void initializeGame()
{
  randomSeed(analogRead(A6)); // using an unused analog pin to randomize the random generator because it's not random actually
  randomNumbersGenerated = 0;
  buttonsPushed = 0;  
  newNumber = 0;
  timerSpeed = 0;
}

void startTheGame()
{
  initializeGame();
  
  #if MusicOn
  myDFPlayer.play(2);

  #endif
  delay(300);
  gameStarted = true;
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
}

void reboot() 
{
  wdt_disable();
  wdt_enable(WDTO_15MS);
}
