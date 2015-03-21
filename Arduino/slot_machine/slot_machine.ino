/* Pin mode
 * RC522      UNO R3
 * Reset      9
 * SPI SS     10
 * SPI MOSI   11
 * SPI MISO   12
 * SPI SCK    13
 * 3.3V       3.3V
 * GND        GND
 * LED 1      1
 * LED 2      2
 * LED 3      3
*/

/* Variable expression
 * 
 */

/* Usage
 * 1. 顯示黃燈，代表沒有卡在上面
 * 2. 將卡放入，顯示綠燈，網頁顯示新的金額
 * 3. 拉下拉桿，網頁扣除金額，
 * 4. 結果顯示輸贏，網頁改變金額，將值寫入RFID，亮紅燈
 * 5. 寫值完成，顯示綠燈，將卡拿出
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
const int buttonPin = 0;

// RFID variable
int block=2;
byte blockcontent[16] = {"255"};
byte readbackblock[18];

// LED variable
int status_card = 1;
char incomingByte;
String txtMsg;

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();        
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
        
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(0, INPUT);
}

void loop(){
    // Step 1
    mfrc522.PCD_Init();
    status_card = 1;
    identifyStatus();
        
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {
       	return;
    }
    
    // Step 2
    status_card = 2;
    identifyStatus();
    readBlock(block, readbackblock);
    Serial.print("read block: ");
    for (int j=0 ; j<16 ; j++){
        Serial.print(readbackblock[j]);
    }
    Serial.println("");
    
    // Step 3
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) {
//        digitalWrite(ledPin, HIGH);  
    } else {
//        digitalWrite(ledPin, LOW); 
    }
    
    // Step 4
    if (Serial.available() > 0) {
        incomingByte = (char) Serial.read();
        if(incomingByte == 'B'){
            txtMsg = "";
            txtMsg += "B";
            Serial.println("Start");
        }else if(incomingByte == 'E'){
//            txtMsg += "E";
            txtMsg = txtMsg.substring(1);
            blockcontent[0] = txtMsg.toInt();
            blockcontent[3] = 100;
            writeBlock(block, blockcontent);
            Serial.println(txtMsg);
            txtMsg = "";
            Serial.println("End");
        }else{
            if(txtMsg.indexOf('B') == 0){
                txtMsg += incomingByte;
                Serial.println(txtMsg);
                Serial.println("Continue");
            }
        }
    }
}

void identifyStatus(){
    switch(status_card){
        case 1:
            digitalWrite(3, LOW);
            digitalWrite(2, LOW);
            digitalWrite(1, HIGH);
            break;
        case 2:
            digitalWrite(3, LOW);
            digitalWrite(2, HIGH);
            digitalWrite(1, LOW);
            break;
        case 3:
            digitalWrite(3, HIGH);
            digitalWrite(2, LOW);
            digitalWrite(1, LOW);
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
            Serial.print(blockNumber);
            Serial.println(" is a trailer block:");
            return 2;
        }
    Serial.print(blockNumber);
    Serial.println(" is a data block:");
  
    byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  
    if (status != MFRC522::STATUS_OK) {
        Serial.print("PCD_Authenticate() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        return 3;//return "3" as error message
    }
    
    status = mfrc522.MIFARE_Write(blockNumber, arrayAddress, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print("MIFARE_Write() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        return 4;//return "4" as error message
    }
    Serial.println("block was written");
}

int readBlock(int blockNumber, byte arrayAddress[]) {
    int largestModulo4Number=blockNumber/4*4;
    int trailerBlock=largestModulo4Number+3;
    
    byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  
    if (status != MFRC522::STATUS_OK) {
        Serial.print("PCD_Authenticate() failed (read): ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        return 3;//return "3" as error message
    }
    
    byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
    status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
    if (status != MFRC522::STATUS_OK) {
        Serial.print("MIFARE_read() failed: ");
        Serial.println(mfrc522.GetStatusCodeName(status));
        return 4;
    }
    
    Serial.println("block was read");
    delay(500);
}
