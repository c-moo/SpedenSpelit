#include "display.h"

const int latchPin = 10;
const int clockPin = 11;
const int dataPin = 8;
const int outEnable = 9;
const int reset = 12;

//luodaan numerot 0-9 hexadesimaaleina
int dataArray[10] = {
  0x3F,  // 0
  0x06,  // 1
  0x5B,  // 2
  0x4F,  // 3
  0x66,  // 4
  0x6D,  // 5
  0x7D,  // 6
  0x07,  // 7
  0x7F,  // 8
  0x6F   // 9
};


void initializeDisplay(void) {
  //Laitetaan digitaalipinnit 8-12 tilaan OUTPUT
  for (int x = 8; x < 13; x++) {
    pinMode(x, OUTPUT);
  }

  //Tämä vakiona korkea
  digitalWrite(reset, HIGH);
}




void writeByte(uint8_t number, bool last) {

  //jos last on true, asetetaan out-enable pinni tilaan HIGH
  if (last) {
    digitalWrite(outEnable, HIGH);
  } else {
    digitalWrite(outEnable, LOW);
  }


  //asetetaan latchpinni LOW tilaan Datan lähettämistä varten varten;
  digitalWrite(latchPin, LOW);

  //Lähetetään numero bitti kerrallaan rekisterille
  for (int i = 0; i < 8; i++) {
    //Shiftataan numeroa bitWise operaatioilla, ja eristetään haluttu bitti 
    int bitti = (dataArray[number] >> (7 - i)) & 0x01;

    digitalWrite(dataPin, bitti);

    //Lähetetään kellosignaali, jotta voidaan lähettää bitit
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(50);  //Annetaan kellopinnille vähän aikaa hoitaa hommansa
    digitalWrite(clockPin, LOW);
    delayMicroseconds(50);
  }
}


void writeHighAndLowNumber(uint8_t tens, uint8_t ones) {

  //Kirjoitetaan ykköset
  writeByte(ones, true);

  //Asetetaan pieni delay luvun kirjoittamisen jälkeen, jotta displayt ehtivät päivittyä.
  delay(5);

  digitalWrite(latchPin, HIGH);  // Latchataan data rekisteriin, ja näytetään numero
  digitalWrite(latchPin, LOW);
  
  //Kirjoitetaan kympit
  writeByte(tens, false);
  delay(5);

  digitalWrite(latchPin, HIGH);  // Latchataan data rekisteriin, ja näytetään numero
  digitalWrite(latchPin, LOW);
}

void showResult(byte result) {
  int tens = 0;
  int ones = 0;

  //erotetaan kympit ja ykköset
  //aamuyön ratkaisuja
  if (result > 9) {
    tens += result % 10;
    tens = (result - tens) / 10;
  }
  ones += result - (tens * 10);

  //Kirjoitetaan kympit ja ykköset
  writeHighAndLowNumber(tens, ones);
}
