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
 
 GND  GND
 VCC  5V
 SDA A4
 SCL A5
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

char incomingByte;
String txtMsg;
long pre_money = 0;
String username = "";
String choice = "";
String bet_money = "";

int game_status = 0; // 0: at first; 1: choose money; 2: choose number; 3: wait for result 

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
    mfrc522.PCD_Init();
        
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
//        Serial.println("It is the same card.");
//        delay(300);
        return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) {
//        Serial.println("Can't read the card.");
//        delay(100);
       	return;
    }
    
    if (Serial.available() > 0) {
        incomingByte = (char) Serial.read();
        Serial.println(incomingByte);
        if(incomingByte == 'R'){
            txtMsg = "";
            txtMsg += "R";
        }else if(incomingByte == 'T'){
          // Get the result number
          txtMsg = txtMsg.substring(1);
          countResult(txtMsg);
        }else{
            if(txtMsg.indexOf('R') == 0){
                txtMsg += incomingByte;
            }
        }
    }
    
    readBlock(block, readbackblock);
    if(pre_money != readbackblock[15]){
//        Serial.println(readbackblock[15]);
        for(int i=9; i<15; i++){
          if(readbackblock[i] != 0){
            char c = char(readbackblock[i]);
            username += c;
//            result[i-9] = readbackblock[i];
          }
        }
        String welcome = format_string("Welcome "+username);
        lcd.setCursor(0,0);
        lcd.print(welcome);
        pre_money = readbackblock[15];
        String money = format_money(pre_money*500);
        lcd.setCursor(0,1);
        lcd.print(money);
    }
    
    char key_input = keypad.getKey();
    
    if(game_status == 0){
//      Serial.println(key_input);
      if(key_input == '#'){
        game_status = 1;

        lcd.setCursor(0,0);
        lcd.print("Type your choice");
        lcd.setCursor(0,1);
        lcd.print("                ");
        key_input = NO_KEY;
      }
    }
    
    if (key_input != NO_KEY){
      if(game_status == 1){
//        Serial.println(key_input);
        switch(key_input){
          case '1':
            lcd.setCursor(0,1);
            lcd.print("1           ");
            choice = "1";
            
            break;
          case '2':
            lcd.setCursor(0,1);
            lcd.print("2           ");
            choice = "2";
            break;
          case '3':
            lcd.setCursor(0,1);
            lcd.print("3           ");
            choice = "3";
            break;
          case '4':
            lcd.setCursor(0,1);
            lcd.print("4           ");
            choice = "4";
            break;
          case '5':
            lcd.setCursor(0,1);
            lcd.print("5           ");
            choice = "5";
            break;
          case '6':
            lcd.setCursor(0,1);
            lcd.print("6           ");
            choice = "6";
            break;
          case '7':
            lcd.setCursor(0,1);
            lcd.print("Small       ");
            choice = "7";
            break;
          case '8':
            lcd.setCursor(0,1);
            lcd.print("Big         ");
            choice = "8";
            break;
          case '#':
            if(choice != ""){
              game_status = 2;
              lcd.setCursor(0,0);
              lcd.print("bet money        ");
              lcd.setCursor(0,1);
              lcd.print("                 ");
              key_input = NO_KEY;
            }
            break;
          default:
            lcd.setCursor(0,1);
            lcd.print("Wrong choice");
            break;
        }
      }
      
      if(game_status == 2){
        lcd.setCursor(0,1);
        switch(key_input){
          case '1':
            bet_money += "1";
            lcd.print(format_string(bet_money));
            break;
          case '2':
            bet_money += "2";
            lcd.print(format_string(bet_money));
            break;
          case '3':
            bet_money += "3";
            lcd.print(format_string(bet_money));
            break;
          case '4':
            bet_money += "4";
            lcd.print(format_string(bet_money));
            break;
          case '5':
            bet_money += "5";
            lcd.print(format_string(bet_money));
            break;
          case '6':
            bet_money += "6";
            lcd.print(format_string(bet_money));
            break;
          case '7':
            bet_money += "7";
            lcd.print(format_string(bet_money));
            break;
          case '8':
            bet_money += "8";
            lcd.print(format_string(bet_money));
            break;
          case '9':
            bet_money += "9";
            lcd.print(format_string(bet_money));
            break;
          case '0':
            if(bet_money != ""){
              bet_money += "0";
              lcd.print(format_string(bet_money));
            }
            break;
          case '*':{
            lcd.setCursor(0,1);
            int ml = bet_money.length();
            bet_money = bet_money.substring(0, ml-1);
            lcd.print(format_string(bet_money));
            lcd.setCursor(ml-1,1);
            break;}
          case '#':
            if((bet_money.toInt() % 500) == 0){
              game_status = 3;
              Serial.println("S");
              key_input = NO_KEY;
              lcd.setCursor(0,0);
              lcd.print(format_string("Wait for result"));
              waitResult();
            }else{
              lcd.setCursor(0,0);
              lcd.print(format_string("500 per unit"));
              delay(1000);
              lcd.setCursor(0,0);
              lcd.print(format_string("bet money"));
            }
            break;
          default:
            break;
        }
      }
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
//    delay(500);
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

String format_string(String string){
  int len = string.length();
  if(len<16){
    for(int i=0; i<16-len; i++){
      string += " ";
    }
  }
  return string;
}

void waitResult(){
  Serial.println("S");
//  delay(2000);
//  int r = random(1, 7);
//  String rand = String(r);
//  long b = bet_money.toInt();
}

void countResult(String result){
  lcd.setCursor(0,0);
  lcd.print(format_string(result));
  delay(2000);
  int r = result.toInt();
  int c = choice.toInt();
  long b = bet_money.toInt();
  if(r == c){
    lcd.setCursor(0, 0);
    lcd.print(format_string("Brovo!"));
    lcd.setCursor(0,1);
    long earn = (b / 500) * 3 + pre_money;
    lcd.print(format_string("You earn " + format_money(earn * 500)));
    writeToRfid(pre_money+3);
//    delay(2000);
//    blockcontent[15] = pre_money+3;
//    writeBlock(block, blockcontent);
  }else if(r <= 3 && choice.toInt() == 7){
    lcd.setCursor(0, 0);
    lcd.print(format_string("Good!"));
    lcd.setCursor(0,1);
    long earn = (b / 500) * 1 + pre_money;
    lcd.print(format_string("You earn " + format_money(earn * 500)));
    writeToRfid(pre_money+1);
//    delay(2000);
//    blockcontent[15] = pre_money+1;
//    writeBlock(block, blockcontent);
  }else if(r >= 4 && choice.toInt() == 8){
    lcd.setCursor(0, 0);
    lcd.print(format_string("Good!"));
    lcd.setCursor(0,1);
    long earn = (b / 500) * 1 + pre_money;
    lcd.print(format_string("You earn " + format_money(earn * 500)));
    writeToRfid(pre_money+1);
//    delay(2000);
//    blockcontent[15] = pre_money+1;
//    writeBlock(block, blockcontent);
  }else{
    lcd.setCursor(0, 0);
    lcd.print(format_string("Try again"));
    lcd.setCursor(0,1);
    long earn = pre_money - (b / 500);
    lcd.print(format_string("You earn " + format_money(earn * 500)));
    writeToRfid(pre_money-1);
//    delay(2000);
//    blockcontent[15] = pre_money-1;
//    writeBlock(block, blockcontent);
  }
}

void writeToRfid(int money_from){
  readBlock(block, readbackblock);
  for(int i=0; i<16; i++){
    blockcontent[i] = readbackblock[i];
  }
  blockcontent[15] = money_from;
  writeBlock(block, blockcontent);
  delay(5000);
}
  
