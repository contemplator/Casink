void setup() {
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A5, OUTPUT);
}

void loop() {
  lightOn();
  delay(1000);
  lightOff();
  delay(1000);
}

void lightOn(){
  for(int i=0; i<=13; i++){
    digitalWrite(i, HIGH);
  }
  digitalWrite(A5, HIGH);
}

void lightOff(){
  for(int i=0; i<=13; i++){
    digitalWrite(i, LOW);
  }
  digitalWrite(A5, LOW);
}

void lightBlue(){
  digitalWrite(13, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(9, HIGH);
}

void lightWhite(){
  digitalWrite(13, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(9, LOW);
}

void lightNothing(){
  digitalWrite(13, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(9, LOW);
}
