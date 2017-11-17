#include <Servo.h>

boolean newData = false;
char receivedChar = 0, hChar, vChar;
Servo servoH, servoV;
int actualPosUpper,actualPosLower;
int CHARMAX,RANGEMAX;

//servo horizontale : lower
//servo verticale : upper
void setup(){
  CHARMAX = 'l' - 'a';
  RANGEMAX = 5;
  Serial.begin(9600);
  servoV.attach(2, 900, 2400);
  servoH.attach(3);
  actualPosUpper = 90;
  actualPosLower = 90;
  servoV.write(actualPosUpper);
  servoH.write(actualPosLower);
}

void loop(){
  receivedChar=0;
  track();
}

void readOneChar() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    newData = true;
  }
}

void track() {
  readOneChar();
  
  int v = 0,h = 0;
  
  if (newData == false)
    return;
    
  if(receivedChar >= 'a' && receivedChar <= 'z'){
    h = receivedChar - 'a';
    h = map(h, 0, CHARMAX, 0, RANGEMAX);

    if(receivedChar < 'l'){
       //actualPosUpper += (int)(receivedChar-'a'); 
       actualPosUpper -= h;
       //actualPosUpper -= 1;
    } else if (receivedChar > 'l') {
       //actualPosUpper -= (int)(receivedChar-'a'); 
       actualPosUpper += h;
//       actualPosUpper += 1;
    }
    servoH.write(actualPosUpper); 

  }
  else if(receivedChar >= 'A' && receivedChar <= 'Z'){
    v = receivedChar - 'A';
    v = map(v, 0, CHARMAX, 0, RANGEMAX);
    
    if(receivedChar < 'L'){
       //actualPosUpper += (int)(receivedChar-'a'); 
       actualPosLower -= 12 - v;
       //actualPosLower -= 1;
    } else if (receivedChar > 'L') {
       //actualPosUpper -= (int)(receivedChar-'a'); 
       actualPosLower += v;
       //actualPosLower += 1;
    }
    servoV.write(actualPosLower); 
  }
  
  newData = false;
}

