/*
 * RC522      UNO R3
 * Reset      9
 * SPI SS     10
 * SPI MOSI   11
 * SPI MISO   12
 * SPI SCK    13
 * 3.3V       3.3V
 * GND        GND
 * slider     A0
*/

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

int block=2;
byte blockcontent[16];
byte readbackblock[18];

int status_card = 1;
char incomingByte;
String txtMsg;
int pre_money = 0;
boolean new_user = true;
int lastState = 0;
String username;

static unsigned long lastDebounceTime;
#define DEBOUNCE_DELAY 500

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();        
  for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
  }
  pinMode(A0, INPUT);
}

void loop(){
  mfrc522.PCD_Init();
  status_card = 1;
      
  if (!mfrc522.PICC_IsNewCardPresent()) {
      new_user = true;
      Serial.println("B0E");
      delay(500);
      return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    new_user = true;
    return;
  }
  
  if(new_user == true){
    readBlock(block, readbackblock);
    username = "";
    for(int i=9; i<15; i++){
      if(readbackblock[i] != 0){
        char c = char(readbackblock[i]);
        username += c;
      }
    }
    Serial.println("#"+username+"$");
    
    String money = (String)readbackblock[15];
    pre_money = money.toInt();
    Serial.println("B" + money + "E");
    new_user = false;
    
  }
  
  status_card = 2;
  
  if(status_card == 2){
    int currentState = analogRead(A0);
//    Serial.println(currentState);
    if(currentState > 600 && currentState < 700){
      sendToNodejs();
    }
  }
  
  if (Serial.available() > 0) {
    incomingByte = (char) Serial.read();
    if(incomingByte == 'B'){
      txtMsg = "";
      txtMsg += "B";
      status_card = 3;
    }else if(incomingByte == 'E'){
      txtMsg = txtMsg.substring(1);
      writeToRfid(txtMsg);
    }else{
      if(txtMsg.indexOf('B') == 0){
          txtMsg += incomingByte;
      }
    }
  } 
}

void sendToNodejs(){
  unsigned long currentTime = millis();
  if((currentTime - lastDebounceTime) > DEBOUNCE_DELAY){
    Serial.print("S"); // start
  }
}

void writeToRfid(String money_from){
  readBlock(block, readbackblock);
  for(int i=0; i<16; i++){
    blockcontent[i] = readbackblock[i];
  }
  blockcontent[15] = money_from.toInt();
  writeBlock(block, blockcontent);
  new_user = true;
  txtMsg = "";
  status_card = 1;
}

int writeBlock(int blockNumber, byte arrayAddress[]){
    status_card = 3;
    int largestModulo4Number=blockNumber/4*4;
    int trailerBlock=largestModulo4Number+3;
        if (blockNumber > 2 && (blockNumber+1)%4 == 0){
            return 2;
        }
  
    byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  
    if (status != MFRC522::STATUS_OK) {
        return 3;//return "3" as error message
    }
    
    status = mfrc522.MIFARE_Write(blockNumber, arrayAddress, 16);
    if (status != MFRC522::STATUS_OK) {
        return 4;//return "4" as error message
    }
}

int readBlock(int blockNumber, byte arrayAddress[]) {
    int largestModulo4Number=blockNumber/4*4;
    int trailerBlock=largestModulo4Number+3;
    
    byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  
    if (status != MFRC522::STATUS_OK) {
        return 3;//return "3" as error message
    }
    
    byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
    status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
    if (status != MFRC522::STATUS_OK) {
        return 4;
    }
}
