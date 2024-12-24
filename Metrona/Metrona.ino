#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

int ResetPin = 4;
int StartStopPin = 5;
int TimeSignaturePin = 7;
int EButtonPin = 6;
int EncoderPinA = 2;
int EncoderPinB = 3;
int BuzzerPin = 9;
int LedPins[] = {13, 12, 8, 10, 11};
LiquidCrystal_I2C lcd(0x27, 16, 2);


void BPM (int bpm, int timesig);
bool isOn = false;
int ledIndex = 0;
int numLeds = sizeof(LedPins) / sizeof(LedPins[0]);
int buzzerLength = 1;

volatile int bpm = 100;
int lastEncoderStateA = LOW;

void setup() {
  //encoder
  pinMode(EncoderPinA, INPUT_PULLUP);
  pinMode(EncoderPinB, INPUT_PULLUP);
  pinMode(EButtonPin, INPUT_PULLUP);

  //set all leds as outputs
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  lcd.begin(16,2);
}   

void loop() {

  ////FIX ME 
  
  int currentStateA = digitalRead(EncoderPinA);
  if (currentStateA != lastEncoderStateA && currentStateA == HIGH) {
    if (digitalRead(EncoderPinB) != currentStateA) {
      bpm++; 
    } else {
      bpm--; 
    }
  bpm = constrain(bpm, 40, 4000);
  lcd.setCursor(0, 1);
  lcd.backlight();
  lcd.print("BPM: ");
  lcd.print(bpm);
  lcd.print("   "); 
  }
  lastEncoderStateA = currentStateA;
  newBPM(bpm, 4); 
}


void newBPM (int bpm, int timesig) {
  int buzzerDelay = 60000 / bpm ;
  int ledDelay = buzzerDelay / numLeds;
  bool ledForward = true;
  buzzerDelay = buzzerDelay - buzzerLength - ledDelay;

  isOn = true;

  if (isOn){
    while (isOn == true) {
      // beep once
      tone(BuzzerPin, 200);
      delay(buzzerLength);
      noTone(BuzzerPin);
      delay(buzzerDelay);

      //move thru leds 
      for (int i = 0; i < (numLeds); i++) {
        digitalWrite(LedPins[ledIndex], HIGH);
        delay(ledDelay / (numLeds)); 
        digitalWrite(LedPins[ledIndex], LOW);
        if (ledForward) {
          ledIndex++;
        } else {
          ledIndex--;
        }
      }

      if (ledIndex == numLeds) {
          ledForward = false; 
          digitalWrite(LedPins[numLeds-1], HIGH);
        } else if (ledIndex == 0) {
          digitalWrite(LedPins[0], HIGH);
          ledForward = true; 
        }

      }

      // beep again
      tone(BuzzerPin, 100);
      delay (buzzerLength);
      noTone(BuzzerPin);
      delay(buzzerDelay);

    
  }
}
