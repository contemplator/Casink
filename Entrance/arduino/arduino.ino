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

/* 
 * LCD  Arduino
 * GND  GND
 * VCC  5V
 * SDA  A4
 * SCL  A5
*/

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define SS_PIN 10
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int block=2;
byte blockcontent[16];
byte readbackblock[18];
char name[6];
char result[7];
byte result2[7];

char incomingByte;
String txtMsg;
String username = "";
String pre_username = "";
String piece[3];

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();        
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print(format_string("Device is ready"));
}

void loop(){
    mfrc522.PCD_Init();
        
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
//        Serial.println("It is the same card.");
        lcd.setCursor(0,0);
        lcd.print(format_string("Device is ready"));
        lcd.setCursor(0,1);
        lcd.print(format_string(""));
        delay(500);
        return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {
//        Serial.println("Can't read the card.");
        delay(500);
       	return;
    }

    lcd.setCursor(0,1);
    lcd.print(format_string("READY"));
      
    if (Serial.available() > 0) {
        incomingByte = (char) Serial.read();
        if(incomingByte == '#'){
          txtMsg = "";
          txtMsg += "#";
        }else if(incomingByte == '$'){
          writeToRfid();
        }else{
          if(txtMsg.indexOf('#') == 0){
            txtMsg += incomingByte;
          }
        }
    }
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
    
    delay(500);
}

void writeToRfid(){
  int lastIndex = 0;
  for(int i=0; i<3; i++){
    if((txtMsg.indexOf(";", lastIndex)) != -1){
      int index = txtMsg.indexOf(";", lastIndex+1);
      piece[i] = txtMsg.substring(lastIndex+1, index);
      lastIndex = index;
    }
  }
  
  // username
  piece[1].toCharArray(result, 7);
  delay(2000);
  for(int i=0; i<sizeof(result); i++){
      result2[i] = result[i];
  }
  for(int i=9; i<15; i++){
      blockcontent[i] = result2[i-9];
  }
  // money
  blockcontent[15] = piece[2].toInt();
  // card id
  int id = piece[0].toInt();
  int group_id = 0;
  int stored_id = 0;
  if(id > 255){
    group_id = id / 255;
    stored_id = id % 255;
  }else{
    stored_id = id;
  }
  blockcontent[7] = group_id;
  blockcontent[8] = stored_id;
  writeBlock(block, blockcontent);

  txtMsg = "";
  for(int i=0; i<sizeof(result2); i++){
    result[i] = 0;
    result2[i] = 0;
  }
  
  lcd.setCursor(0,0);
  lcd.print(format_string("Write done"));
  Serial.print("done");
}

String format_string(String string){
  int len = string.length();
  if(len<16){
    for(int i=0; i<16-len; i++){
      string += " ";
    }
  }
  return string;
}
