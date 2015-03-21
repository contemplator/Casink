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
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

int block=2;
byte blockcontent[16];
byte readbackblock[18];
char name[6];
char result[6];
byte result2[6];

char incomingByte;
String txtMsg;
String username = "";
String pre_username = "";

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();        
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}

void loop(){
    mfrc522.PCD_Init();
        
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
        Serial.println("It is the same card.");
        pre_username = "";
        delay(500);
        return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {
        Serial.println("Can't read the card.");
        delay(500);
       	return;
    }
    
    readBlock(block, readbackblock);
    username = "";
    for(int i=9; i<15; i++){
      char n = char(readbackblock[i]);
      if(readbackblock[i] != 0){
        username = username + n;
      }
    }
    
    if(pre_username != username){
      Serial.print("Hello ");
      Serial.println(username);
      pre_username = username;
    }

    
    if (Serial.available() > 0) {
        incomingByte = (char) Serial.read();
        
        if(incomingByte == '#'){
          txtMsg = "";
          txtMsg += "#";
        }else if(incomingByte == '$'){
          txtMsg = txtMsg.substring(1);
          txtMsg.toCharArray(result, 6);
          
          for(int i=0; i<sizeof(result); i++){
              result2[i] = result[i];
          }
          
          for(int i=9; i<15; i++){
              blockcontent[i] = result2[i-9];
          }
          blockcontent[15] = 10;
          writeBlock(block, blockcontent);
          Serial.println("Write done");
          txtMsg = "";
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
