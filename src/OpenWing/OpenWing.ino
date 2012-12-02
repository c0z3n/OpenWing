// OpenWing.ino - arduino sketch which comprises the firmware of the OpenWing device
// full repository located at github.com/c0z3n/OpenWing
// contact gordon.p.clement@gmail.com
//
//     Copyright (C) Gordon Clement 2012
//
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with this program.  If not, see http://www.gnu.org/licenses.

#include <SPI.h>
#include <Ethernet.h>

#define WIRELESS_SEND  true  //set true to stream control data over the wireless connection (if wireless radio is present)
#define USB_SEND       false //set true to stream control data over the usb connection
#define OSC_SEND       true  //set true to send control data as OSC over ethernet IP network (if ethernet hardware is present)



const int myIp[]           = {192, 168, 10, 210};
const byte myMac[]         = {0x90, 0xA2, 0xDA, 0x0D, 0x18, 0x24};
const int faders[]         = {4, 5, 6, 7, 8, 9, 10, 11};
const int colorpins[][3]   = {{12, 13, 11}, {8, 9, 7}, {5, 6, 4}, {2, 3, 44}};
const int buttons[]        = {A0, A1, A2, A3};

int  colorvals[][3]        = {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}};
byte lastbuttons[]         = {0, 0, 0, 0};
byte button_states[]       = {0, 0, 0, 0};
int  fader_raws[]          = {0, 0, 0, 0, 0, 0, 0, 0};
byte fader_vals[]          = {0, 0, 0, 0, 0, 0, 0, 0};
byte last_fader_vals[]     = {0, 0, 0, 0, 0, 0, 0, 0};

byte trackXp               = 0;
byte trackYp               = 0;
byte trackXm               = 0;
byte trackYm               = 0;

void setup(){
  analogReference(EXTERNAL);
  if(WIRELESS_SEND){Serial2.begin(9600);}
  if(USB_SEND){Serial.begin(9600);}
  for(int i=0; i<4; i++){
    for(int n=0; n<3; n++){
      pinMode(colorpins[i][n], OUTPUT);
      analogWrite(colorpins[i][n], 255);
    }
  }
  for(int i=0; i<4; i++){
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH); // enable internal pullup
  }
  for(int i=0; i<8; i++){
    fader_raws[i] = analogRead(faders[i]);
    fader_vals[i] = last_fader_vals[i] = map(fader_raws[i], 0, 1023, 0, 255);
  }
  attachInterrupt(2, trackLeft, CHANGE);
  attachInterrupt(3, trackRight, CHANGE);
  attachInterrupt(4, trackUp, CHANGE);
  attachInterrupt(5, trackDown, CHANGE);
}

void loop(){
  for(int i=0; i<4; i++){
  // check the state of each button
    if(digitalRead(buttons[i]) == LOW){
      button_states[i] = 1;
    }
    else{
      button_states[i] = 0;
    }
  }
  // check the fader states and map them to the correct range
  for(int i=0; i<8; i++){
    fader_raws[i] = analogRead(faders[i]);
    fader_vals[i] = map(fader_raws[i], 0, 1023, 0, 255);
  }
  // read incoming serial messages on wireless connection
  if(WIRELESS_SEND){
    recieve_wireless();
  }
  // read incoming serial messages on usb (wired) connection
  if(USB_SEND){
    recieve_usb();
  }
  set_colors();
  send_data();
  delay(25);
}

void set_colors(){
// handle setting led colors
  for(int i=0; i<4; i++){
    for(int n=0; n<3; n++){
      if(button_states[i] == 1){
        // if a button is pressed, override the color with white.
        analogWrite(colorpins[i][0], 0);
        analogWrite(colorpins[i][1], 0);
        analogWrite(colorpins[i][2], 0);
      }
      else{
        // else use the color as defined in colorvals
        analogWrite(colorpins[i][0], colorvals[i][0]);
        analogWrite(colorpins[i][1], colorvals[i][1]);
        analogWrite(colorpins[i][2], colorvals[i][1]);
      }
    }
  }
}
void send_data(){
  // send the states of all buttons and faders over serial
  Serial2.write("dx");
  for(int i=0; i<4; i++){
    Serial2.write(button_states[i]);
  }
  for(int i=0; i<8; i++){
    Serial2.write(fader_vals[i]);
  }
  Serial2.write(trackXp);
  Serial2.write(trackYp);
  Serial2.write(trackXm);
  Serial2.write(trackYm);
  trackXm = trackYm = trackXp = trackYp = 0;
}

void recieve_usb(){
  if(Serial.available() > 1){
    byte c1 = Serial.read();
    byte c2 = Serial.read();
    // check for the alignment key
    if(c1 == 'l' && c2 == 'x'){
      byte lednum = Serial.read();
      byte r = Serial.read();
      byte g = Serial.read();
      byte b = Serial.read();
      colorvals[lednum][0] = int(r);
      colorvals[lednum][1] = int(g);
      colorvals[lednum][2] = int(b);
    }
  }
}
void recieve_wireless(){
  if(Serial2.available() > 1){
    byte c1 = Serial2.read();
    byte c2 = Serial2.read();
    // check for the alignment key
    if(c1 == 'l' && c2 == 'x'){
      byte lednum = Serial2.read();
      byte r = Serial2.read();
      byte g = Serial2.read();
      byte b = Serial2.read();
      colorvals[lednum][0] = int(r);
      colorvals[lednum][1] = int(g);
      colorvals[lednum][2] = int(b);
    }
  }
}

void trackUp(){
  trackYp = trackYp + 1;
}
void trackDown(){
  trackYm = trackYm + 1;
}
void trackLeft(){
  trackXp = trackXp + 1;
}
void trackRight(){
  trackXm = trackXm + 1;
}
