#include "Arduino.h"
#include "DFRobotDFPlayerMini.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

int ResetPin = 9;
int StartStopPin = 10;

int TimeSignaturePin = 5;
int BPMButtonPin = 4;
int BPMPinA = 2;
int BPMPinB = 3;

int VolumePin = A0;

int BuzzerPin = 6;
int LedPin = 13;

LiquidCrystal_I2C lcd( 0x27, 16, 2);
SoftwareSerial Speaker(11,12);
DFRobotDFPlayerMini DFPlayer;

void setup() {

}   

void loop() {

}
