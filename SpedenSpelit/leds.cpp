#include "leds.h"


void initializeLeds() //alustaa ledit
{
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
    clearAllLeds();
// see requirements for this function from leds.h
}

void setLed(byte ledNumber) // asetta led numerolle 0 paikan A2, ledi 1 paikan A3.. jne 
{
    clearAllLeds();  
    switch (ledNumber) {
        case 0: digitalWrite(A2, HIGH); break;
        case 1: digitalWrite(A3, HIGH); break;
        case 2: digitalWrite(A4, HIGH); break;
        case 3: digitalWrite(A5, HIGH); break;
        default: break;
    }
    Serial.print("-");                      //debugausta varten
    Serial.println(ledNumber);
// see requirements for this function from leds.h
}


void clearAllLeds() // sammuttaa kaikki ledit
{
// see requirements for this function from leds.h
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
}

void setAllLeds() //laittaa kaikki ledit päälle
{
    //ledit päälle
    digitalWrite(A2, HIGH);
    digitalWrite(A3, HIGH);
    digitalWrite(A4, HIGH);
    digitalWrite(A5, HIGH);
}


void show1()    //näyttää luvut 0-15 binäärinä ledeillä 
{
// see requirements for this function from leds.h
// 
//for loop että voimme toistaa ledejä
    for (int i = 0; i < 16; i++) { 
        digitalWrite(A2, (i & 0b0001) ? HIGH : LOW);
        digitalWrite(A3, (i & 0b0010) ? HIGH : LOW);
        digitalWrite(A4, (i & 0b0100) ? HIGH : LOW);
        digitalWrite(A5, (i & 0b1000) ? HIGH : LOW);

        delay(100); // sadan ms viive
    }
    clearAllLeds(); // ledit pois
}

void show2(int rounds)
{
// see requirements for this function from leds.h  
       int muuttuvaViive = 500; // viive aluksi, sillä voimme nopeuttaA myöhemmin

    for (int r = 0; r < rounds; r++) { // kierroksien toistaminen
        for (byte led = 0; led < 4; led++) { // ledien sytytys vuorotellen
            clearAllLeds(); // ledit pois
            switch (led) {
                case 0: digitalWrite(A2, HIGH); break;
                case 1: digitalWrite(A3, HIGH); break;
                case 2: digitalWrite(A4, HIGH); break;
                case 3: digitalWrite(A5, HIGH); break;
            }
            delay(muuttuvaViive); // viive ennen seuraavan ledin syttymistä
        }
        muuttuvaViive = max(50, muuttuvaViive - 50); // viiven nopeuitus
    }
    clearAllLeds(); // ledit pois
}