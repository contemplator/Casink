/*
 * RC522      UNO R3
 * Reset      9
 * SPI SS     10
 * SPI MOSI   11
 * SPI MISO   12
 * SPI SCK    13
 * 3.3V       3.3V
 * GND        GND
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

char incomingByte;
boolean new_user = true;
String msg;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  SPI.begin();
  mfrc522.PCD_Init();        
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop(){
  mfrc522.PCD_Init();
  
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
//    Serial.println("same");
    Serial.println("BOE");
    delay(500);
    new_user = true;
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    new_user = true;
//    Serial.println("error");
    return;
  }
    
  if(new_user == true){
    readBlock(block, readbackblock);
    int money = readbackblock[15];
    String sendData = "B";
    sendData += money;
    sendData += "E";
    Serial.println(sendData);
//    Serial.print("B");
//    Serial.println(money);
//    Serial.print("E");
    new_user = false;
  }
  
  if(new_user == false){
    if (Serial.available() > 0) {
      incomingByte = (char) Serial.read();
//      Serial.println(incomingByte);
      if(incomingByte == 'W' || incomingByte == 'L'){
        msg += incomingByte;
      }else if(incomingByte == 'E'){
        countResult();
//        minusMoney();
//        addMoney();
      }else{
        msg += incomingByte;
      }
    }
  }
}

void addMoney(int bet){
  // add 5*500 dollars into user's card
  digitalWrite(2, HIGH);
  readBlock(block, readbackblock);
  for(int i=0; i<sizeof(blockcontent); i++){
    blockcontent[i] = readbackblock[i];
  }
  blockcontent[15] += (bet*5);
  writeBlock(block, blockcontent);
  digitalWrite(13, HIGH);
  new_user = true;
  msg = "";
}

void minusMoney(int bet){
  // miuns 1*500 dollars from user's card
  digitalWrite(2, LOW);
  readBlock(block, readbackblock);
  for(int i=0; i<sizeof(blockcontent); i++){
    blockcontent[i] = readbackblock[i];
  }
  blockcontent[15] -= bet;
  writeBlock(block, blockcontent);
  digitalWrite(13, LOW);
  new_user = true;
  msg = "";
}

int writeBlock(int blockNumber, byte arrayAddress[]){
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
    return 4; //return "4" as error message
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

void countResult(){
  boolean isWin = false;
  if(msg.indexOf("W") >= 0){
    isWin = true;
  }
  int bet = msg.substring(1).toInt();
  if(isWin == true){
    addMoney(bet);
  }else{
    minusMoney(bet);
  }
  Serial.println(msg);
}
