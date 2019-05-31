/*********************************************************
 * Eolium Firmware 2.0.6 USB capacitive touch
 * Author: Grégory CHAPITEAU
 * Website : http://eaonsynergy.fr/
 */


/*********************************************************
 * MIDIUSB_test.ino
 *
 * Created: 4/6/2015 10:47:08 AM
 * Author: gurbrinder grewal
 * Modified by Arduino LLC (2015)
 */ 

//MIDI_CREATE_DEFAULT_INSTANCE();
#include "MIDIUSB.h"

//Global variables
int buttonAPin = 2;
int buttonBPin = 3;
int buttonCPin = 4;
int buttonDPin = 5;
int buttonEPin = 6;
int buttonFPin = 7;
int buttonAltPin = 15;

int buttonFXPin = 14;

int buttonOctUpPin = 8;
int buttonOctDownPin = 9;

int pressedButtons = 0;
bool noteOnStatus = false;

//Setup pressure sensor
int pressureSensorPin = A0;
int pressureSensorValue = 0;

//Setup getMidiPressure
int Atm = 0;
int maxPressureSensorValue = 1024;
int midiPressure = 0;
int gainVelocity=5;
int gainAftertouch = 3;
int pressureGate = 3;
int sampling = 10;

//MIDI setup
int midiChannel = 1;
int oldPitch = 0;
int pitch = 0; //for test and debug
int fx = 0;

void setup() {
  Serial.begin(115200);
  
  pinMode(buttonAPin, INPUT);
  pinMode(buttonBPin, INPUT);
  pinMode(buttonCPin, INPUT);
  pinMode(buttonDPin, INPUT);
  pinMode(buttonEPin, INPUT);
  pinMode(buttonFPin, INPUT);
  pinMode(buttonAltPin, INPUT);

  pinMode(buttonFXPin, INPUT);
  
  pinMode(buttonOctUpPin, INPUT);
  pinMode(buttonOctDownPin, INPUT);

  digitalWrite(buttonAPin, HIGH);
  digitalWrite(buttonBPin, HIGH);
  digitalWrite(buttonCPin, HIGH);
  digitalWrite(buttonDPin, HIGH);
  digitalWrite(buttonEPin, HIGH);
  digitalWrite(buttonFPin, HIGH);
  digitalWrite(buttonAltPin, HIGH);

  digitalWrite(buttonFXPin, HIGH);

  digitalWrite(buttonOctUpPin, HIGH);
  digitalWrite(buttonOctDownPin, HIGH);
  
  //Get normal atmosphere pressure value
  Atm = analogRead(pressureSensorPin);

  
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

//Convert pressure sensor valur out from DAC to MIDI value
int getMidiPressure(int pressureSensorValue, int gainMode) {
  int midiPressure = 0;
  
  switch(gainMode){
    case 1: //apply Velocity gain
      midiPressure = (127*(Atm - pressureSensorValue)/(Atm - maxPressureSensorValue)) * gainVelocity;
      break;
    case 2: //apply Aftertouch gain
      midiPressure = (127*(Atm - pressureSensorValue)/(Atm - maxPressureSensorValue)) * gainAftertouch;
      break;
    }
  
  if (midiPressure >127){ //limit to midi compliant value
    return 127;
    }
  else {
    return midiPressure;
  }
}

//Convert note buttons combination into MIDI pitch value
int getMidiPitch(){
  pressedButtons = 
      (1-digitalRead(buttonAPin))
    + (1-digitalRead(buttonBPin)) * 2
    + (1-digitalRead(buttonCPin)) * 4
    + (1-digitalRead(buttonDPin)) * 8
    + (1-digitalRead(buttonEPin)) * 16
    + (1-digitalRead(buttonFPin)) * 32
    + (1-digitalRead(buttonAltPin)) * 64;
  
switch(pressedButtons){
  case 0: pitch = 71; break;
  case 1: pitch = 69; break;
  case 3: pitch = 67; break;
  case 5: pitch = 65; break;
  case 7: pitch = 64; break;
  case 11: pitch = 62; break;
  case 15: pitch = 60; break;
  case 16: pitch = 59; break;
  case 17: pitch = 57; break;
  case 19: pitch = 55; break;
  case 21: pitch = 53; break;
  case 23: pitch = 52; break;
  case 27: pitch = 50; break;
  case 31: pitch = 48; break;
  case 32: pitch = 47; break;
  case 33: pitch = 45; break;
  case 35: pitch = 43; break;
  case 37: pitch = 41; break;
  case 39: pitch = 40; break;
  case 43: pitch = 38; break;
  case 47: pitch = 36; break;
  case 48: pitch = 35; break;
  case 49: pitch = 33; break;
  case 51: pitch = 31; break;
  case 53: pitch = 29; break;
  case 55: pitch = 28; break;
  case 59: pitch = 26; break;
  case 63: pitch = 24; break;
  case 64: pitch = 70; break;
  case 65: pitch = 68; break;
  case 67: pitch = 66; break;
  case 69: pitch = 65; break;
  case 71: pitch = 63; break;
  case 75: pitch = 61; break;
  case 79: pitch = 60; break;
  case 80: pitch = 58; break;
  case 81: pitch = 56; break;
  case 83: pitch = 54; break;
  case 85: pitch = 53; break;
  case 87: pitch = 51; break;
  case 91: pitch = 49; break;
  case 95: pitch = 48; break;
  case 96: pitch = 46; break;
  case 97: pitch = 44; break;
  case 99: pitch = 42; break;
  case 101: pitch = 41; break;
  case 103: pitch = 39; break;
  case 107: pitch = 37; break;
  case 111: pitch = 36; break;
  case 112: pitch = 34; break;
  case 113: pitch = 32; break;
  case 115: pitch = 30; break;
  case 117: pitch = 29; break;
  case 119: pitch = 27; break;
  case 123: pitch = 25; break;
  case 127: pitch = 24; break;
  
  default: pitch = oldPitch; break;
  }

  return pitch;
  //return 62;
  }

void loop() {
  pressureSensorValue = analogRead(pressureSensorPin);
  midiPressure = getMidiPressure(pressureSensorValue, 1);
  pitch=getMidiPitch();

  fx = 1-digitalRead(buttonFXPin);

  if(fx == 1){
      midiPressure = getMidiPressure(pressureSensorValue, 2);
      controlChange(midiChannel, 2, midiPressure);
    }
  else {
      controlChange(midiChannel, 2, 0);   //Reset aftertouch on channel
    }
  
  if (midiPressure > pressureGate){
    switch (noteOnStatus){
      case true: if(pitch == oldPitch){
          //
        }
        else{
          delay(sampling);
          noteOff(midiChannel, oldPitch, midiPressure);
          MidiUSB.flush();

          delay(sampling);
          pressureSensorValue = analogRead(pressureSensorPin);
          midiPressure = getMidiPressure(pressureSensorValue, 1);
          noteOn(midiChannel, pitch, midiPressure);   //Note On new pitch
          MidiUSB.flush();
          oldPitch = pitch;
          }
        
        midiPressure = getMidiPressure(pressureSensorValue, 1);
        delay(sampling);
        break;
      case false: //Note ON
        delay(sampling);
        pressureSensorValue = analogRead(pressureSensorPin);
        midiPressure = getMidiPressure(pressureSensorValue, 1);
        noteOn(midiChannel, pitch, midiPressure);   
        MidiUSB.flush();
        oldPitch = pitch;
        Serial.print("NoteOn - Pitch:");
        Serial.print(pitch);
        Serial.print(" - Pressure: ");
        Serial.print(midiPressure);
        Serial.print(" - Channel: ");
        Serial.print(midiChannel);
        Serial.print(" - Pressed: ");
        Serial.print(pressedButtons);
        Serial.print("\n");
        noteOnStatus = true;
        break;
        } 
  }
  
  if(noteOnStatus == true && midiPressure < pressureGate){ //Note OFF if no breath
        
        noteOff(midiChannel, oldPitch, midiPressure);
        
        MidiUSB.flush();
        Serial.print("NoteOff - Pitch:");
        Serial.print(pitch);
        Serial.print(" - Pressure: ");
        Serial.print(midiPressure);
        Serial.print(" - Channel: ");
        Serial.print(midiChannel);
        Serial.print("\n");
        noteOnStatus = false;
        delay(sampling);
    }
          
}
