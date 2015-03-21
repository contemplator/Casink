#include <SoftwareSerial.h>
#include <Wire.h>

#define MAX_BTCMDLEN 128

SoftwareSerial BTSerial(10,11); // Arduino RX/TX
byte cmd[MAX_BTCMDLEN]; // received 128 bytes from an Android system
int len = 0; // received command length

void setup() {
    Serial.begin(9600);   // Arduino起始鮑率：9600
    BTSerial.begin(9600); // HC-06 出廠的鮑率：每個藍牙晶片的鮑率都不太一樣，請務必確認
}

void loop() {
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
        Serial.println(str);
        cmd[0] = '\0';
    }
    len = 0;
}
