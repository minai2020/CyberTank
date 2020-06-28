
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <Keypad.h>
HUSKYLENS huskylens;
const byte ROWS = 4; 
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3}; 
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

SoftwareSerial mySerial(10, 11);
void printResult(HUSKYLENSResult result);
int rf=14;
int rb=15;
int lf=16;
int lb=17;
int enr=12;
int enl=13;

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    pinMode (rf, OUTPUT);
    pinMode (rb, OUTPUT);
    pinMode (lf, OUTPUT);
    pinMode (lb, OUTPUT);
    pinMode (enr, OUTPUT);
    pinMode (enl, OUTPUT);

    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
}

void loop() 
{
  
    if (!huskylens.request()) 
Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));

    else if(!huskylens.isLearned()) 
Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));

    else if(!huskylens.available()) 
Serial.println(F("No block or arrow appears on the screen!"));

    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
            driveBot(result);
        }    
    }
}


void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}

void driveBot(HUSKYLENSResult result)
{
char customKey = customKeypad.getKey();

if(customKey==1){
  if((result.ID==1)&&(result.xCenter<=140))
  {
    left();
  }

  else if((result.ID==1)&&(result.xCenter>=200))
  {
    right();
  }

    else if((result.ID==1)&&(result.xCenter>=140)&&(result.xCenter<=200))
  {
    if((result.ID==1)&&(result.width<=20))
    {
      forward();
    }

    else if((result.ID==1)&&(result.width>20))
    {
      backward();
    }
  }
  
}
else if(customKey==2){
  if((result.ID==1)&&(result.width<20)){
    trightforward();
  }
  if((result.ID==2)&&(result.width<20)){
    tleftforward();
  }
}
}

//------------------------------------------------------------------------motor control code for object tracking or human following---------------------------------------------------------------
//check all the PWM
void right()
{
digitalWrite(rf, LOW);
digitalWrite(rb, HIGH);
digitalWrite(lf, HIGH);
digitalWrite(lb, LOW);
analogWrite(enr, 200);
analogWrite(enl, 200);
}
void left()
{
digitalWrite(rf, HIGH);
digitalWrite(rb, LOW);
digitalWrite(lf, LOW);
digitalWrite(lb, HIGH);
analogWrite(enr, 200);
analogWrite(enl, 200);
}

void forward()
{
digitalWrite(rf, HIGH);
digitalWrite(rb, LOW);
analogWrite(enr, 200);
digitalWrite(lf, HIGH);
digitalWrite(lb, LOW);
analogWrite(enl, 200);
}

void backward()
{
digitalWrite(rf, LOW);
digitalWrite(rb, HIGH);
analogWrite(enr, 200);
digitalWrite(lf, LOW);
digitalWrite(lb, HIGH);
analogWrite(enl, 200);
}

//--------------------------------------------------------------------------------motor control code for tag navigation-------------------------------------------------------------------------
//check the delay and pwm
void trightforward()
{
digitalWrite(rf, LOW);
digitalWrite(rb, HIGH);
digitalWrite(lf, HIGH);
digitalWrite(lb, LOW);
analogWrite(enr, 200);
analogWrite(enl, 200);
delay(500);
digitalWrite(rf, HIGH);
digitalWrite(rb, LOW);
analogWrite(enr, 200);
digitalWrite(lf, HIGH);
digitalWrite(lb, LOW);
analogWrite(enl, 200);
}

void tleftforward()
{
digitalWrite(rf, HIGH);
digitalWrite(rb, LOW);
digitalWrite(lf, LOW);
digitalWrite(lb, HIGH);
analogWrite(enr, 200);
analogWrite(enl, 200);
delay(500);
digitalWrite(rf, HIGH);
digitalWrite(rb, LOW);
analogWrite(enr, 200);
digitalWrite(lf, HIGH);
digitalWrite(lb, LOW);
analogWrite(enl, 200);
}
