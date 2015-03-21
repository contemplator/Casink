/*
 * RC522      UNO R3
 * Reset      9
 * SPI SS     10
 * SPI MOSI   11
 * SPI MISO   12
 * SPI SCK    13
 * 3.3V       3.3V
 * GND        GND
 * LED 1      2
 * LED 2      3
 * LED 3      4
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

int status_card = 1;
char incomingByte;
String txtMsg;
int pre_money = 0;
boolean new_user = true;
const int buttonPin = 7;
int buttonState = 0;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();        
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
        
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(buttonPin, INPUT);
}

void loop(){
    mfrc522.PCD_Init();
    status_card = 1;
    identifyStatus();
        
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
        new_user = true;
        return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {
        new_user = true;
       	return;
    }
    
    if(new_user == true){
        readBlock(block, readbackblock);
        String money = (String)readbackblock[15];
        Serial.println("B" + money + "E");
        new_user = false;
    }
    
    status_card = 2;
    identifyStatus();
    
    if(status_card == 2){
        buttonState = digitalRead(buttonPin);
        if (buttonState == 1) {
            Serial.println("BSE");
        }else{
        }
    }else{
      
    }
    
    if (Serial.available() > 0) {
        incomingByte = (char) Serial.read();
        if(incomingByte == 'B'){
            txtMsg = "";
            txtMsg += "B";
            status_card = 3;
            identifyStatus();
        }else if(incomingByte == 'E'){
            txtMsg = txtMsg.substring(1);
            blockcontent[15] = txtMsg.toInt();
            writeBlock(block, blockcontent);
            String money = (String)blockcontent[15];
            Serial.println("B" + money + "E");
            new_user = true;
            txtMsg = "";
            status_card = 1;
            identifyStatus();
        }else{
            if(txtMsg.indexOf('B') == 0){
                txtMsg += incomingByte;
            }
        }
    }
    
    delay(500);
    
//    readBlock(block, readbackblock);
//    if(pre_money != readbackblock[14]){
//        Serial.println(readbackblock[14]);
//        pre_money = readbackblock[14];
//    }
//    Serial.print("read block: ");
//    for (int j=0 ; j<16 ; j++){
//        Serial.write(readbackblock[j]);
//        Serial.println(readbackblock[14]);
//    }
//    Serial.println("");
}

void identifyStatus(){
    switch(status_card){
        case 1:
            digitalWrite(4, LOW);
            digitalWrite(3, LOW);
            digitalWrite(2, HIGH);
            break;
        case 2:
            digitalWrite(4, LOW);
            digitalWrite(3, HIGH);
            digitalWrite(2, LOW);
            break;
        case 3:
            digitalWrite(4, HIGH);
            digitalWrite(3, LOW);
            digitalWrite(2, LOW);
            break;
        default:
            break;
    }
}

int writeBlock(int blockNumber, byte arrayAddress[]){
    status_card = 3;
    identifyStatus();
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
//    delay(500);
}
