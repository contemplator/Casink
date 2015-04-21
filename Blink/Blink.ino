void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  delay(200);
//  lightOn(10);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  delay(200);
//  lightOn(11);
  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  delay(200);
//  lightOn(12);
  digitalWrite(12, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(9, LOW);
  digitalWrite(13, LOW);
  delay(200);
//  lightOn(13);
  digitalWrite(13, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(9, LOW);
  delay(200);
}

void lightOn(int pin){
  for(int i=9; i<=13; i++){

    if(pin == i){
    Serial.println(pin);
      digitalWrite(pin, HIGH);
    }else{
      digitalWrite(pin, LOW);
    }
  }
}
