#include <SPI.h>
#include <Ethernet.h>

// configuration stuff, pins, ethernet information, etc.
const int myIp[]           = {192, 168, 10, 210};
const byte myMac[]         = {0x90, 0xA2, 0xDA, 0x0D, 0x18, 0x24};
const int faders[]         = {4, 5, 6, 7, 8, 9, 10, 11};
const int colorpins[][3]   = {{12, 13, 11}, {8, 9, 7}, {5, 6, 4}, {2, 3, 44}};
const int buttons[]        = {A0, A1, A2, A3};

// the initial color values for the buttons. 
// colors are inverted to what you might expect, (255, 255, 255) is LED off.
int  colorvals[][3]        = {{255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}};

//variables to keep track of the buttons
byte lastbuttons[]         = {0, 0, 0, 0};
byte button_states[]       = {0, 0, 0, 0};

//variables to keep track of the fader values and re-mapped fader values.
int  fader_raws[]          = {0, 0, 0, 0, 0, 0, 0, 0};
byte fader_vals[]          = {0, 0, 0, 0, 0, 0, 0, 0};
byte last_fader_vals[]     = {0, 0, 0, 0, 0, 0, 0, 0};

byte fader_flags[]         = {0, 0, 0, 0, 0, 0, 0, 0};
byte button_flags[]        = {0, 0, 0, 0};

byte fader_IDs[]           = {1, 2, 3, 4, 5, 6, 7, 8};
byte button_IDs[]          = {9, 10, 11, 12};

byte packet_open           = 0;

void setup(){
  analogReference(EXTERNAL);
//  Serial2.begin(9600);
  Serial.begin(115200);
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
    fader_vals[i] = last_fader_vals[i] = fader_raws[i] >> 2;
  }
//  attachInterrupt(2, trackLeft, CHANGE);
//  attachInterrupt(3, trackRight, CHANGE);
//  attachInterrupt(4, trackUp, CHANGE);
//  attachInterrupt(5, trackDown, CHANGE);
}

void loop(){
  for(int i=0; i<4; i++){
  // check the state of each button
    lastbuttons[i] = button_states[i];
    if(digitalRead(buttons[i]) == LOW){
      button_states[i] = 1;
    }
    else{
      button_states[i] = 0;
    }
    if(button_states[i] != lastbuttons[i]){
      button_flags[i] = 1;
    }
    else{
      button_flags[i] = 0;
    }
  }
  // check the fader states and map them to the correct range
  for(int i=0; i<8; i++){
    fader_flags[i] = 0;
    fader_raws[i] = analogRead(faders[i]);
    last_fader_vals[i] = fader_vals[i];
    fader_vals[i] = fader_raws[i] >> 2;
    if (fader_vals[i] == last_fader_vals[i]){
      fader_flags[i] = 0;
    }
    else{
      fader_flags[i] = 1;
    }
  }
  // read incoming serial messages
//  if(Serial2.available() > 1){
//    byte c1 = Serial2.read();
//    byte c2 = Serial2.read();
//    // check for the alignment key
//    if(c1 == 'l' && c2 == 'x'){
//      byte lednum = Serial2.read();
//      byte r = Serial2.read();
//      byte g = Serial2.read();
//      byte b = Serial2.read();
//      colorvals[lednum][0] = int(r);
//      colorvals[lednum][1] = int(g);
//      colorvals[lednum][2] = int(b);
//    }
//    else{
//      while(Serial2.available() > 0){
//        byte temp = Serial2.read();
//      }
//      Serial2.flush();
//    }
//  }
//  set_colors();
  broadcast(Serial);
  delay(5);
}

void broadcast(HardwareSerial &com){
  byte sent_start = 0;
  byte bytecount = get_bytecount();
  
  for(int i=0; i<8; i++){
    if(fader_flags[i] == 1){
      send_value(com, bytecount, fader_IDs[i], fader_vals[i]);
    }
  }
  for(int i=0; i<4; i++){
    if(button_flags[i] == 1){
      send_value(com, bytecount, button_IDs[i], button_states[i]);
    }
  }
  packet_open = 0;
}

byte get_bytecount(){
  byte count = 0;
  for(int i=0; i<8; i++){
    if(fader_flags[i] == 1){
      count++;
    }
  }
  for(int i=0; i<4; i++){
    if(button_flags[i] == 1){
      count++;
    }
  }
  return count;
}

void send_value(HardwareSerial &com, byte bytecount, byte id, byte value){
  if(packet_open == 0){
    com.write(byte(0));
    com.write(byte(0));
    com.write(bytecount);
    packet_open = 1;
  }
  com.write(id);
  com.write(value);
}

