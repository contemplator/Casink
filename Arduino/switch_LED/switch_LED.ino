static int ledStatus;
static unsigned long lastDebounceTime;
#define DEBOUNCE_DELAY 500

void setup(){
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);
  ledStatus = LOW;
  digitalWrite(13, ledStatus);
}

void updateLed(){
  unsigned long currentTime = millis();
  if((currentTime - lastDebounceTime) > DEBOUNCE_DELAY){
    lastDebounceTime = currentTime;
    ledStatus = ledStatus == HIGH ? LOW : HIGH;
    digitalWrite(13, ledStatus);
    Serial.print("S"); // meaning the code is used for switch
    Serial.print(ledStatus);
    Serial.print("E"); // meaning end
  }
}

void loop(){
  int switchStatus = digitalRead(2);
  if(switchStatus == HIGH){
    updateLed();
  }
}
