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


bool isOn = false;
int ledIndex = 0;
int numLeds = sizeof(LedPins) / sizeof(LedPins[0]);
int buzzerLength = 1;

volatile int bpm = 100; 
volatile bool bpmChanged = false;

// testing new encoder function
//volatile bool lastStateA = 0;
//volatile bool lastStateB = 0;

volatile int lastEncoded = 0;
volatile int encoderValue = 0;

void BPM (int bpm, int timesig);
void encoder();

bool stateA = digitalRead(EncoderPinA);
bool stateB = digitalRead(EncoderPinB);

void setup() {
  //for debugging
  Serial.begin(9600);
  Serial.print("-----------------------------------------------------------------");

  //encoder set up
  pinMode(EncoderPinA, INPUT_PULLUP);
  pinMode(EncoderPinB, INPUT_PULLUP);
  pinMode(EButtonPin, INPUT_PULLUP);

  //set up encoder interrupts
  attachInterrupt(digitalPinToInterrupt(EncoderPinA), encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncoderPinB), encoder, CHANGE);

  //turn on lcd
  lcd.begin(16, 2);
  lcd.backlight();
 
  //set all leds as outputs
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
}   

void loop() {
  //for debugging
  Serial.print("current BPM  : ");
  Serial.println(bpm);


  Serial.print("A: ");
  Serial.println(stateA);

  Serial.print("B: ");
  Serial.println(stateB);

  if (bpmChanged) {
    lcd.setCursor(0, 1);
    lcd.print("BPM: ");
    lcd.print(bpm);
    lcd.print("   ");
    //newBPM(bpm, 0);
    bpmChanged = false; // Reset 
  }
  delay(1000);
}

void encoder() {
  stateA = digitalRead(EncoderPinA);
  stateB = digitalRead(EncoderPinB);
  ///FIX ME 
  if (stateA != lastStateA) { // Only process when there's a change in stateA
    if (stateB != stateA) {
      bpm++; // Clockwise
    } else {
      bpm--; // Counter-clockwise
    }
    bpm = constrain(bpm, 40, 150); // Limit BPM range
    bpmChanged = true; 

    Serial.print("BPM updated to : ");
    Serial.println(bpm);
  }

  lastStateA = stateA; // Update the last state of EncoderPinA
  
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

