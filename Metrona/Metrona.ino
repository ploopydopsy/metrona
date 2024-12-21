#include "Arduino.h"
#include <LiquidCrystal_I2C.h>

int ResetPin = 4;
int StartStopPin = 5;
int TimeSignaturePin = 7;
int BPMButtonPin = 6;
int BPMPinA = 2;
int BPMPinB = 3;
int VolumePin = A0;
int BuzzerPin = 9;

int LedPins[] = {8, 10, 11, 12, 13};

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

}   

void loop() {

}
