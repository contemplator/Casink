#include <Keypad.h>

/*
 left: close number 1 on the keypad
 pin digital 2 : left 1
 pin digital 3 : left 2
 pin digital 4 : left 3
 pin digital 5 : left 4
 pin digital 6 : left 5
 pin digital 7 : left 6
 pin digital 8 : left 7
*/

const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'#','0','*'},
  {'9','8','7'},
  {'6','5','4'},
  {'3','2','1'}
};
byte rowPins[rows] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[cols] = {8, 7, 6}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
int analogInPin = 0;
int sensorValue = 0;

void setup(){
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop(){
  char key = keypad.getKey();

  if (key != NO_KEY){
    Serial.println(key);
  }
  
  if (Serial.available()) {
    int val = Serial.read() -48;
    if (val != 0){
      digitalWrite(13, HIGH);
      Serial.println(val);
    }else{
      digitalWrite(13, LOW);
      Serial.println(val);
    }
  }
  
  sensorValue = analogRead(analogInPin);
  sensorValue = map(sensorValue, 0, 674, 0, 255);
  Serial.println(sensorValue);
  delay(100);
  
}
