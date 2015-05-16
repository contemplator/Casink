// BLE   Arduino
// TX    Pin5
// RX    Pin6

#include <SoftwareSerial.h>
#include <Time.h>

SoftwareSerial BTSerial(5, 6); // TX / RX

//static int ledStatus;
static unsigned long lastDebounceTime;
#define DEBOUNCE_DELAY 500
boolean state = false;

void setup() {
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop(){
//  BTSerial.print("1");
//  Serial.print("1");
//  delay(1000);
//  BTSerial.print("0");
//  Serial.print("0");
//  delay(1000);
  
  int switchStatus = digitalRead(2);
  if(switchStatus == HIGH){
    unsigned long currentTime = millis();
    if((currentTime - lastDebounceTime) > DEBOUNCE_DELAY){
      lastDebounceTime = currentTime;
      state = !state;
      judgeState();
    }
  }
  
  if(BTSerial.available()){
    char tmpChar = BTSerial.read();
    if(tmpChar == '1'){
      digitalWrite(13, HIGH);
    }else if(tmpChar == '0'){
      digitalWrite(13, LOW);
    }
  }
}

void judgeState(){
  if(state == true){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }
}
