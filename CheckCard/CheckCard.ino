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
//char name[6];
//char result[6];
//byte result2[6];

//char incomingByte;
//String txtMsg;
//String username = "";
//String pre_username = "";
//String piece[3];

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
        Serial.println("It is the same card.");
        delay(500);
        return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {
        Serial.println("Can't read the card.");
        delay(500);
       	return;
    }
    
    readBlock(block, readbackblock);//read the block back
    for (int i=0 ; i<16 ; i++){
        delay(1000);
        lcd.setCursor(0,0);
        String number = String(i);
        lcd.print(format_string(number));
        lcd.setCursor(0,1);
        String text;
        if(readbackblock[i] >= 48){
            text += char(readbackblock[i]);
            lcd.print(format_string(text));
            text = "";
        }else{
          lcd.print(format_string(String(readbackblock[i])));
        }
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

String format_string(String string){
  int len = string.length();
  if(len<16){
    for(int i=0; i<16-len; i++){
      string += " ";
    }
  }
  return string;
}
