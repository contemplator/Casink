int sensorPin1 = A0; // white
int ledPin1 = 13;
int sensorValue1 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A3, OUTPUT);
}

void loop() {
  sensorValue1 = analogRead(sensorPin1);
  Serial.print("1:");
  Serial.println(sensorValue1);
  if(sensorValue1 > 1000){
    onBlue();
    offWhite();
  }else if(sensorValue1 > 500){
    onWhite();
    offBlue();
  }else{
    offWhite();
    offBlue();
  }
}

void onWhite(){
  digitalWrite(12, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(A4, HIGH);
}

void offWhite(){
  digitalWrite(12, LOW);
  digitalWrite(10, LOW);
  digitalWrite(7, LOW);
  digitalWrite(5, LOW);
  digitalWrite(2, LOW);
  digitalWrite(A4, LOW);
}

void onBlue(){
//  Serial.println("asjl");
  digitalWrite(13, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(A5, HIGH);
  digitalWrite(A3, HIGH);
}

void offBlue(){
  digitalWrite(13, LOW);
  digitalWrite(11, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(6, LOW);
  digitalWrite(4, LOW);
  digitalWrite(3, LOW);
  digitalWrite(A5, LOW);
  digitalWrite(A3, LOW);
}
