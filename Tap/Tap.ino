int sensorPin1 = A0; // white
//int sensorPin2 = A1;
int ledPin1 = 13;
//int ledPin2 = 12;
int sensorValue1 = 0;
//int sensorValue2 = 2;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
//  pinMode(ledPin2, OUTPUT);
}

void loop() {
  sensorValue1 = analogRead(sensorPin1);
//  sensorValue2 = analogRead(sensorPin2);
  Serial.print("1:");
  Serial.println(sensorValue1);
//  Serial.print("2:");
//  Serial.println(sensorValue2);
  if(sensorValue1 > 500){
    digitalWrite(ledPin1, HIGH);
  }else{
    digitalWrite(ledPin1, LOW);
  }
  
//  if(sensorValue2 > 500){
//    digitalWrite(ledPin2, HIGH);
//  }else{
//    digitalWrite(ledPin2, LOW);
//  }
  
//  delay(200);
}
