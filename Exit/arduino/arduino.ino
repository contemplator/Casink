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
long userid = 0;
String username = "";
long money = 0;
String result = "";
String currentData = "";
String sendData = "";

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
        delay(500);
        return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {
//        Serial.println("Can't read the card.");
        delay(500);
       	return;
    }
    
    readBlock(block, readbackblock); //read the block back
    userid = readbackblock[8] + readbackblock[7]*255;
    
    for(int i=9; i<15; i++){
      if(readbackblock[i] >= 48){
        String name = String(i);
        username += char(readbackblock[i]);
      }
    }
    
    money = readbackblock[15] * 500;
    
    if(money < 50000){
      result = "Poor man";
    }else if(money> 50000 && money<100000){
      result = "normal man";
    }else{
      result = "Rich man";
    }
    
    lcd.setCursor(0,0);
    lcd.print(format_string("Are you " + username + " ?"));
    lcd.setCursor(0,1);
    lcd.print(format_string("You own " + String(money) + " ."));
    delay(2000);
    lcd.setCursor(0,0);
    lcd.print(format_string("You are " + result + "."));
    lcd.setCursor(0,1);
    lcd.print(format_string("HaHaHa"));
    delay(2000);
    
    currentData = "#" + String(userid) + ";" + String(money/500) + "$";
    if(currentData != sendData){
      Serial.print(currentData);
      sendData = currentData;
    }
    
    username = "";
    money = 0;
    result = "";
    
//    delay(500);
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

String format_string(String string){
  int len = string.length();
  if(len<16){
    for(int i=0; i<16-len; i++){
      string += " ";
    }
  }
  return string;
}
