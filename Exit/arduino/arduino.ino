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
        lcd.setCursor(0,0);
        lcd.print(format_string("Device is ready"));
        lcd.setCursor(0,1);
        lcd.print(format_string(""));
        delay(500);
        return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {
//        Serial.println("Can't read the card.");
        lcd.setCursor(0,0);
        lcd.print(format_string("Something is wrong!"));
        lcd.setCursor(0,1);
        lcd.print(format_string(""));
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
    
    money = long(readbackblock[15]) * 500;
    
    currentData = "#" + String(userid) + ";" + String(money/500) + "$";
//    if(currentData != sendData){
      Serial.print(currentData);
      sendData = currentData;
//    }
    
    displayResult();
    
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

String format_money(long m){
//  Serial.println(money);
  String final = "$";
  String money = String(m);
//  Serial.println(money);
  int length = money.length();
  if(length > 6){
//    Serial.println("6");
    money = money.substring(0, (length-6))+ "," + money.substring((length-6), (length-3)) + "," + money.substring((length-3), length);
  }else if(length > 3){
    money = money.substring(0, (length-3)) + "," + money.substring((length-3), (length));
  }
  final = final + money;
  final = format_string(final);
  return final;
}

void displayResult(){
    lcd.setCursor(0,0);
    lcd.print(format_string("Are you " + username + " ?"));
    lcd.setCursor(0,1);
    lcd.print(format_string("You own " + format_money(money) ));
    delay(2000);
    
    if(money <= 5000){
      lcd.setCursor(0,0);
      lcd.print(format_string("Such a Poor Guy."));
      scrolling("HaHaHa XDDD");
    }else if(money > 5000 && money <= 10000){
      lcd.setCursor(0,0);
      lcd.print(format_string("Just a Normal man."));
      scrolling("/ _> \\");
    }else if(money > 10000 && money <= 20000){
      lcd.setCursor(0,0);
      lcd.print(format_string("Just lucky.."));
      scrolling("Do you wnat to challenge again?");
    }else if(money > 20000 && money <= 50000){
      lcd.setCursor(0,0);
      lcd.print(format_string("WOW"));
      scrolling("You are the destiny person!");
    }else if(money > 50000){
      lcd.setCursor(0,0);
      lcd.print(format_string("Unbelievable"));
      scrolling("Congraduation! You are best!");
    }
//    delay(3000);
}

void scrolling(String message){
  int length = message.length();
  if(length > 16){
    for(int j=0; j<2; j++){
      for(int i=0; i<(length - 16); i++){
        lcd.setCursor(0,1);
        lcd.print(format_string(message.substring(i, i+16)));
        delay(300);
      }
      
      for(int i=(length-16); i>= 0; i--){
        lcd.setCursor(0,1);
        lcd.print(format_string(message.substring(i, i+16)));
        delay(300);
      }
    }
  }else{
    lcd.setCursor(0,1);
    lcd.print(format_string(message));
    delay(3000);
  }
}
