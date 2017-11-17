#include <Servo.h>

boolean newData = false;
char receivedChar = 0;
Servo servoH, servoV;
int actualPosUpper,actualPosLower;

//servo horizontale : lower
//servo verticale : upper
void setup(){
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
  readOneChar();
  //showNewData();
  convertAdvance(receivedChar);
  
}

void readOneChar() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    //convert(receivedChar);
    newData = true;
  }
}

void showNewData() {
  if (newData == true) {
    switch(receivedChar) {
    case 'z' :
      servoV.write(180);
      break;
    case 's' :
      servoV.write(0);
      break;
    case 'q' :
      servoH.write(180);
      break;
    case 'd' :
      servoH.write(0);
      break;
      case ' ':
      servoH.write(90);
      servoV.write(90);
      break;
    }
    newData = false;
  }
} 

void convert(char c){
  int i;
  Serial.print(c);
  Serial.print("    En binaire :");
  for(i = 7 ; i >= 0 ; i--){
    Serial.print (bitRead(c,i));
  }
  Serial.println();
  delay(1000);
}
// Décompose un caractère en 8 valeurs binaires, se référé à la doc (feuille)
void convertAdvance(char c){
  if (newData == true) {
    int val = c;
    Serial.println(val);
    int i;
    int angleUpperMotor = 0;
    int angleLowerMotor = 0;
    for(i = 7; i > 4; i--){
      angleUpperMotor += ((pow(2,(i-4)))*bitRead(c,i));
      Serial.print("valeur incrementation  H:   ");
      Serial.println(pow(2,((i-4)))*bitRead(c,i));
      Serial.print("Pow(2,2) : ");
      Serial.println(pow(2,2));
      Serial.print("bitRead : ");
      Serial.println(bitRead(c,i));
      Serial.print("i :   ");
      Serial.println(i);
      Serial.print("i - 4 : ");
      Serial.println(i-4);
      Serial.print("AngleupperMotor :   ");
      Serial.println(angleUpperMotor );
    }
    angleUpperMotor *= pow((-1),bitRead(c,4));
    for(i = 3; i > 0; i--){
      angleLowerMotor += pow(2,i)*bitRead(c,i);
      Serial.print("valeur incrementation  L:   ");
      Serial.println(pow(2,i)*bitRead(c,i));
    }
    angleLowerMotor *= pow((-1),bitRead(c,0));
  
    if((actualPosUpper + angleUpperMotor) < 0 || (actualPosUpper + angleUpperMotor) > 180){
          // ERROR
    }
    else{r
        actualPosUpper += angleUpperMotor;
        Serial.print("upperServo : ");
        Serial.println(angleUpperMotor);
    }
    if((actualPosLower + angleLowerMotor) < 0 || (actualPosLower + angleLowerMotor) > 180){
      // ERROR
    }
    else{
        actualPosLower += angleLowerMotor;
        Serial.print("LowerServo : ");
        Serial.println(angleLowerMotor);
    }  
    servoV.write(actualPosUpper);
    servoH.write(actualPosLower);
    newData = false;
  }
  
}
