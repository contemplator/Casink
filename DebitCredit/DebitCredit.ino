// BLE   Arduino
// TX    Pin5
// RX    Pin6

/* 
 * LCD  Arduino
 * GND  GND
 * VCC  5V
 * SDA  A4
 * SCL  A5
*/

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

#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// the configuation of RFID and LCD
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int block=2;
byte blockcontent[16];
byte readbackblock[18];

// the maximum received command length from an Android system (over the bluetooth)
#define MAX_BTCMDLEN 128

SoftwareSerial BTSerial(2,3);

byte cmd[MAX_BTCMDLEN]; // received 128 bytes from an Android system
int len = 0; // received command length

void setup() {
  Serial.begin(9600); // Arduino起始鮑率：9600
  
  // init bluetooth
  BTSerial.begin(9600); // HC-06 出廠的鮑率：每個藍牙晶片的鮑率都不太一樣，請務必確認
  
  // init RFID
  SPI.begin();
  mfrc522.PCD_Init();        
  for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
  }
  
  // init lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(format_string("Device is ready"));
}

void loop() {
  mfrc522.PCD_Init();
        
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
//    Serial.println("It is the same card.");
    lcd.setCursor(0, 0);
    lcd.print(format_string("Device is ready"));
    lcd.setCursor(0, 1);
    lcd.print(format_string(""));
    delay(500);
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
//    Serial.println("Can't read the card.");
    lcd.setCursor(0, 0);
    lcd.print(format_string("Wait for a minute"));
    lcd.setCursor(0, 1);
    lcd.print(format_string(""));
    delay(500);
    return;
  }
  
  // first time read the data in RFID
  readBlock(block, readbackblock); //read the block back
  lcd.setCursor(0, 0);
  lcd.print(format_string("Now, you have:"));
  lcd.setCursor(0, 1);
  lcd.print(format_string(String(readbackblock[15]*500)));
  
  char str[MAX_BTCMDLEN];
  int insize, ii;  
  int tick=0;
  while ( tick<MAX_BTCMDLEN ) { // 因為包率同為9600, Android送過來的字元可能被切成數份
    if ( (insize=(BTSerial.available()))>0 ){ // 讀取藍牙訊息
      for ( ii=0; ii<insize; ii++ ){
        cmd[(len++)%MAX_BTCMDLEN]=char(BTSerial.read());
      }
    } else {
      tick++;
    }
  }

  if ( len ) { // 用串列埠顯示從Android手機傳過來的訊息
    sprintf(str,"%s",cmd);
//    Serial.println(str);
    String receive_data = "";
    for(int i=0; i<MAX_BTCMDLEN; i++){
      if(cmd[i] != 0){
        receive_data += char(cmd[i]);
      }
    }
    
    writeToRFID(receive_data);
//    if(test == "M10Y"){
//      Serial.println("hi");
//    }else{
//      Serial.println("lo");
//    }
    cmd[0] = '\0';
  }

  len = 0;
}

void writeToRFID(String data){
  String test = "";
  for(int i=1; i<data.length()-1; i++){
    test += (char)data[i];
  }
  
//  test += (char)data;
  int money = test.toInt();
  Serial.println(money);
  int lastIndex = 0;
  
  for(int i=0; i<sizeof(blockcontent); i++){
    blockcontent[i] = readbackblock[i];
  }
  blockcontent[15] = blockcontent[15] + money;
  writeBlock(block, blockcontent);

  lcd.setCursor(0,0);
  lcd.print(format_string("Write done"));
//  Serial.print("done");
  delay(3000);
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
