/*********************************************************
 * Eaonium Firmware 2.0.6 USB capacitive touch
 * Author: Grégory CHAPITEAU
 * Website : http://eaonsynergy.fr/
 */

/*********************************************************
This is a library for the MPR121 12-channel Capacitive touch sensor

Designed specifically to work with the MPR121 Breakout in the Adafruit shop 
  ----> https://www.adafruit.com/products/

These sensors use I2C communicate, at least 2 pins are required 
to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution
**********************************************************/

/*
 * MIDIUSB_test.ino
 *
 * Created: 4/6/2015 10:47:08 AM
 * Author: gurbrinder grewal
 * Modified by Arduino LLC (2015)
 */ 

//MIDI_CREATE_DEFAULT_INSTANCE();
#include "MIDIUSB.h"

#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

//Global variables

int buttonNoteStart = 0;
int buttonNoteStop = 9;

int buttonFXPin = 10;

//int buttonOctUpPin = 11;
//int buttonOctDownPin = 12;

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
int pressureGate = 2;
//int sampling = 0; //aftertouch refresh each X milliseconds

//MIDI setup
int midiChannel = 1;
int oldPitch = 0;
int pitch = 0; //for test and debug
int fx = 0;

void setup() {
  Serial.begin(115200);

  //while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  //}
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
  
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

void getPressedPads(){
    // Get the currently touched pads
  currtouched = cap.touched();
  
  for (uint8_t i=buttonNoteStart; i<=buttonNoteStop; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      //Serial.print(i); Serial.println(" touched");
      pressedButtons = pressedButtons + _BV(i);
      //Serial.print("Valeur _BV(i): "); Serial.println(_BV(i));
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      //Serial.print(i); Serial.println(" released");
      pressedButtons = pressedButtons - _BV(i);
    }   
  }

  if ((currtouched & _BV(buttonFXPin))) {
      //Serial.print(i); Serial.println(" touched");
      fx = 1;
      //Serial.println("FX ON");
    }
    if (!(currtouched & _BV(buttonFXPin))) {
      //Serial.println("FX OFF");
      fx = 0;
    } 

  //Serial.println(pressedButtons);
  
  // reset our state
  lasttouched = currtouched;
  
  return;
  }

//Convert note buttons combination into MIDI pitch value
int getMidiPitch(){

  
switch(pressedButtons){
case 0: pitch = 69; break;
case 1: pitch = 67; break;
case 3: pitch = 65; break;
case 7: pitch = 64; break;
case 15: pitch = 62; break;
case 31: pitch = 60; break;
case 63: pitch = 59; break;
case 256: pitch = 57; break;
case 257: pitch = 55; break;
case 259: pitch = 53; break;
case 263: pitch = 52; break;
case 271: pitch = 50; break;
case 287: pitch = 48; break;
case 319: pitch = 47; break;
case 512: pitch = 45; break;
case 513: pitch = 43; break;
case 515: pitch = 41; break;
case 519: pitch = 40; break;
case 527: pitch = 38; break;
case 543: pitch = 36; break;
case 575: pitch = 35; break;
case 768: pitch = 33; break;
case 769: pitch = 31; break;
case 771: pitch = 29; break;
case 775: pitch = 28; break;
case 783: pitch = 26; break;
case 799: pitch = 24; break;
case 831: pitch = 23; break;
case 64: pitch = 68; break;
case 65: pitch = 66; break;
case 67: pitch = 65; break;
case 71: pitch = 63; break;
case 79: pitch = 61; break;
case 95: pitch = 60; break;
case 127: pitch = 58; break;
case 320: pitch = 56; break;
case 321: pitch = 54; break;
case 323: pitch = 53; break;
case 327: pitch = 51; break;
case 335: pitch = 49; break;
case 351: pitch = 48; break;
case 383: pitch = 46; break;
case 576: pitch = 44; break;
case 577: pitch = 42; break;
case 579: pitch = 41; break;
case 583: pitch = 39; break;
case 591: pitch = 37; break;
case 607: pitch = 36; break;
case 639: pitch = 34; break;
case 832: pitch = 32; break;
case 833: pitch = 30; break;
case 835: pitch = 29; break;
case 839: pitch = 27; break;
case 847: pitch = 25; break;
case 863: pitch = 24; break;
case 895: pitch = 22; break;
  
  default: pitch = oldPitch; break;
  }

  return pitch;
  }

void loop() {
  pressureSensorValue = analogRead(pressureSensorPin);
  midiPressure = getMidiPressure(pressureSensorValue, 1);
  getPressedPads();
  pitch=getMidiPitch();

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
          //delay(sampling);
          noteOff(midiChannel, oldPitch, midiPressure);
          MidiUSB.flush();

          //delay(sampling);
          pressureSensorValue = analogRead(pressureSensorPin);
          midiPressure = getMidiPressure(pressureSensorValue, 1);
          noteOn(midiChannel, pitch, midiPressure);   //Note On new pitch
          MidiUSB.flush();
          oldPitch = pitch;
          }
        
        midiPressure = getMidiPressure(pressureSensorValue, 1);
        //delay(sampling);
        break;
      case false: //Note ON
        //delay(sampling);
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
        //delay(sampling);
    }
          
}
