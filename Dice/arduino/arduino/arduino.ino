/*
 * RC522      UNO R3
 * Reset      9
 * SPI SS     10
 * SPI MOSI   11
 * SPI MISO   12
 * SPI SCK    13
 * 3.3V       3.3V
 * GND        GND
 
 left: close number 1 on the keypad
 pin digital 2 : left 1
 pin digital 3 : left 2
 pin digital 4 : left 3
 pin digital 5 : left 4
 pin digital 6 : left 5
 pin digital 7 : left 6
 pin digital 8 : left 7
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

#define SS_PIN 10
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
LiquidCrystal_I2C lcd(0x27,16,2);

const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'#','0','*'},
  {'9','8','7'},
  {'6','5','4'},
  {'3','2','1'}
};
byte rowPins[rows] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[cols] = {8, 7, 6}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

int block=2;
byte blockcontent[16];
byte readbackblock[18];
char name[6];

char incomingByte;
String txtMsg;
int pre_money = 0;

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
    lcd.print("Device is ready");
}

void loop(){
    char key = keypad.getKey();
    mfrc522.PCD_Init();
        
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
        Serial.println("It is the same card.");
        delay(300);
        return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {
        Serial.println("Can't read the card.");
        delay(100);
       	return;
    }
    
    if (Serial.available() > 0) {
        incomingByte = (char) Serial.read();
        if(incomingByte == 'B'){
            txtMsg = "";
            txtMsg += "B";
        }else if(incomingByte == 'E'){
            txtMsg = txtMsg.substring(1);
            blockcontent[14] = txtMsg.toInt();
            writeBlock(block, blockcontent);
            Serial.println(txtMsg);
            txtMsg = "";
        }else{
            if(txtMsg.indexOf('B') == 0){
                txtMsg += incomingByte;
            }
        }
        
        if(incomingByte == '#'){
          txtMsg = "";
          txtMsg += "#";
        }else if(incomingByte == '$'){
          Serial.println(txtMsg.substring(1).length());
          txtMsg = txtMsg.substring(0,1);
          blockcontent[15] = txtMsg.toInt();
          writeBlock(block, blockcontent);
          txtMsg = "";
        }else{
          if(txtMsg.indexOf('#') == 0){
            txtMsg += incomingByte;
          }
        }
    }
    
    readBlock(block, readbackblock);
    if(pre_money != readbackblock[14]){
        Serial.println(readbackblock[14]);
        lcd.setCursor(0,0);
        lcd.print("Welcome ");
        lcd.print(readbackblock[14]);
        Serial.println(readbackblock[15]);
        lcd.setCursor(0,1);
        lcd.print(readbackblock[15]);
        pre_money = readbackblock[14];
    }
    
    if (key != NO_KEY){
      Serial.println(key);
      lcd.print(key);
    }
//    Serial.print("read block: ");
//    for (int j=0 ; j<16 ; j++){
//        Serial.write(readbackblock[j]);
//        Serial.println(readbackblock[14]);
//    }
//    Serial.println("");
}

int writeBlock(int blockNumber, byte arrayAddress[]){
    int largestModulo4Number=blockNumber/4*4;
    int trailerBlock=largestModulo4Number+3;
        if (blockNumber > 2 && (blockNumber+1)%4 == 0){
//            Serial.print(blockNumber);
//            Serial.println(" is a trailer block:");
            return 2;
        }
//    Serial.print(blockNumber);
//    Serial.println(" is a data block:");
  
    byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  
    if (status != MFRC522::STATUS_OK) {
//        Serial.print("PCD_Authenticate() failed: ");
//        Serial.println(mfrc522.GetStatusCodeName(status));
        return 3;//return "3" as error message
    }
    
    status = mfrc522.MIFARE_Write(blockNumber, arrayAddress, 16);
    if (status != MFRC522::STATUS_OK) {
//        Serial.print("MIFARE_Write() failed: ");
//        Serial.println(mfrc522.GetStatusCodeName(status));
        return 4;//return "4" as error message
    }
//    Serial.println("block was written");
}

int readBlock(int blockNumber, byte arrayAddress[]) {
    int largestModulo4Number=blockNumber/4*4;
    int trailerBlock=largestModulo4Number+3;
    
    byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  
    if (status != MFRC522::STATUS_OK) {
//        Serial.print("PCD_Authenticate() failed (read): ");
//        Serial.println(mfrc522.GetStatusCodeName(status));
        return 3;//return "3" as error message
    }
    
    byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
    status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
    if (status != MFRC522::STATUS_OK) {
//        Serial.print("MIFARE_read() failed: ");
//        Serial.println(mfrc522.GetStatusCodeName(status));
        return 4;
    }
    
//    Serial.println("block was read");
    delay(500);
}
