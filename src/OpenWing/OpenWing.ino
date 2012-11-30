// Gordon Clement
// OpenWing Test Sketch
//

#include <SPI.h>
#include <Ethernet.h>

const int FMAX = 255;

const int myIp[]   = { 192, 168, 10, 210 };
const byte myMac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x18, 0x24 };
const int buttons[] = {A0, A1, A2, A3 };
int lastbuttons[] = {false, false, false, false};
const int faders[] = {4, 5, 6, 7, 8, 9, 10, 11};
const int colorpins[][3] = {{12, 13, 11}, {8, 9, 7}, {5, 6, 4}, {2, 3, 44}};
int BSTATES[] = {false, false, false, false};
int fader_raws[] = {0,0,0,0,0,0,0,0};
byte fader_vals[] = {0,0,0,0,0,0,0,0};
byte last_fader_vals[] = {0,0,0,0,0,0,0,0};
const int tball[] = {18, 19, 20, 21};

//these keep track of the trackball position
int tX = 0;
int tY = 0;


void setup(){
  analogReference(EXTERNAL);  
  Serial2.begin(9600);  
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
   attachInterrupt(tball[0], trackUp, CHANGE);
   attachInterrupt(tball[1], trackDown, CHANGE);
   attachInterrupt(tball[2], trackLeft, CHANGE);
   attachInterrupt(tball[3], trackRight, CHANGE);
   
   for(int i=0; i<8; i++){
     fader_raws[i] = analogRead(faders[i]);
     fader_vals[i] = last_fader_vals[i] = map(fader_raws[i], 0, 1023, 0, FMAX);     
   }
   
}

void loop(){
  // read in the button states and handle activity lights
     for(int i=0; i<4; i++){
       if(digitalRead(buttons[i]) == LOW){
         BSTATES[i] = true;
         digitalWrite(colorpins[i][0], 0);
         digitalWrite(colorpins[i][1], 0);
         digitalWrite(colorpins[i][2], 0);
       }
       else{
         BSTATES[i] = false;
         digitalWrite(colorpins[i][0], 255);
         digitalWrite(colorpins[i][1], 255);
         digitalWrite(colorpins[i][2], 255);
       } 
     }
  // read the fader states and map them to the correct range
     for(int i=0; i<8; i++){
       fader_raws[i] = analogRead(faders[i]);
       fader_vals[i] = map(fader_raws[i], 0, 1023, 0, FMAX);
     }
     
     
 // this next section is going to handle sending the button messages    
     //button0
     if(BSTATES[0] == true && lastbuttons[0] == false){
       Serial2.println("b[0]=True");
       lastbuttons[0] = true;
     }
     if(BSTATES[0] == false && lastbuttons[0] == true){
       Serial2.println("b[0]=False");
       lastbuttons[0] = false;
     }     
     //button1
     if(BSTATES[1] == true && lastbuttons[1] == false){
       Serial2.println("b[1]=True");
       lastbuttons[1] = true;
     }
     if(BSTATES[1] == false && lastbuttons[1] == true){
       Serial2.println("b[1]=False");
       lastbuttons[1] = false;
     }     
     //button2
     if(BSTATES[2] == true && lastbuttons[2] == false){
       Serial2.println("b[2]=True");
       lastbuttons[2] = true;
     }
     if(BSTATES[2] == false && lastbuttons[2] == true){
       Serial2.println("b[2]=False");
       lastbuttons[2] = false;
     }     
     //button3
     if(BSTATES[3] == true && lastbuttons[3] == false){
       Serial2.println("b[3]=True");
       lastbuttons[3] = true;
     }
     if(BSTATES[3] == false && lastbuttons[3] == true){
       Serial2.println("b[3]=False");
       lastbuttons[3] = false;
     } 
 // now we're going to do trackball messages
     if(tX != 0){
       String comstring = "tX+=" + String(tX);
       Serial2.println(comstring);
       tX = 0;
     }
     if(tY != 0){
       String comstring = "tY+=" + String(tY);
       Serial2.println(comstring);
       tY = 0;
     }
  // and now the fader messages     
  for(int i=0; i<8; i++){
//    if(fader_vals[i] != last_fader_vals[i]){
    if(fader_vals[i] < last_fader_vals[i]-1 | fader_vals[i] > last_fader_vals[i]+1){
      send_faders();
      break;
//      String fnum = String(i);
//      String comstring = "f[" + fnum + "]=" + String(fader_vals[i]);
//      Serial2.println(comstring);
//      last_fader_vals[i] = fader_vals[i];
    }
  }
  
//  delay(100);
}
void send_faders(){
  String comstring = "f=[";
  for(int i=0; i<8; i++){
    last_fader_vals[i] = fader_vals[i];
    comstring += String(fader_vals[i]) + ",";
  }
  comstring += "]";
  Serial2.println(comstring);
}




// these are our interrupt functions for the trackball
void trackUp(){
  tY = tY+1;
}

void trackDown(){
  tY = tY-1;
}

void trackLeft(){
  tX = tX-1;
}

void trackRight(){
  tX = tX+1;
}


