// Gordon Clement
// OpenWing Test Sketch
//

#include <SPI.h>
#include <Ethernet.h>
#include <ArdOSC.h>

const int myIp[]   = { 192, 168, 10, 210 };
const byte myMac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x18, 0x24 };
const int buttons[] = {A0, A1, A2, A3 };
int lastbuttons[] = {false, false, false, false};
//const int colorpins[][4] = {{12, 8, 5, 2}, {13, 9, 6, 3}, {11, 7, 4, 44}};
const int colorpins[][3] = {{12, 13, 11}, {8, 9, 7}, {5, 6, 4}, {2, 3, 44}};
int BSTATES[] = {false, false, false, false};


void setup(){
  
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
}

void loop(){
   //for(int i=0; i<4; i++){
   //  for(int n=0; n<3; n++){
   //    analogWrite(colorpins[i][n], 0);
   //    delay(100);
   //    analogWrite(colorpins[i][n], 255);
   //  }
   //}
     String command = "";  
     for(int i=0; i<4; i++){
       if(digitalRead(buttons[i]) == LOW){
         BSTATES[i] = true;
         digitalWrite(colorpins[i][1], 0);
         
       }
       else{
         BSTATES[i] = false;
         digitalWrite(colorpins[i][1], 255);
       } 
     }
     
     
     //button0
     if(BSTATES[0] == true && lastbuttons[0] == false){
       Serial2.println("self.buttons[0] = True");
       lastbuttons[0] = true;
     }
     if(BSTATES[0] == false && lastbuttons[0] == true){
       Serial2.println("self.buttons[0] = False");
       lastbuttons[0] = false;
     }
     
     //button1
     if(BSTATES[1] == true && lastbuttons[1] == false){
       Serial2.println("self.buttons[1] = True");
       lastbuttons[1] = true;
     }
     if(BSTATES[1] == false && lastbuttons[1] == true){
       Serial2.println("self.buttons[1] = False");
       lastbuttons[1] = false;
     }
     
     //button2
     if(BSTATES[2] == true && lastbuttons[2] == false){
       Serial2.println("self.buttons[2] = True");
       lastbuttons[2] = true;
     }
     if(BSTATES[2] == false && lastbuttons[2] == true){
       Serial2.println("self.buttons[2] = False");
       lastbuttons[2] = false;
     }
     
     //button3
     if(BSTATES[3] == true && lastbuttons[3] == false){
       Serial2.println("self.buttons[3] = True");
       lastbuttons[3] = true;
     }
     if(BSTATES[3] == false && lastbuttons[3] == true){
       Serial2.println("self.buttons[3] = False");
       lastbuttons[3] = false;
     }
     
     
     
     
     Serial2.println("print 'exectest'");
     //delay();
/*
     int val = digitalRead(buttons[0]);
     if(val==HIGH){
       digitalWrite(colorpins[0][0], 0);
     }
     else{
       digitalWrite(colorpins[0][0], 255);
     }
        */
   
 
   
}

