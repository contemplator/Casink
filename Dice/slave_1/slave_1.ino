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
 pin digital A1 : left 1
 pin digital A0 : left 2
 pin digital 4 : left 3
 pin digital 5 : left 4
 pin digital 6 : left 5
 pin digital 7 : left 6
 pin digital 8 : left 7
 
 GND  GND
 VCC  5V
 SDA A4
 SCL A5
 
 BLE Master
 TX    Pin2
 RX    Pin3
 
 BLE Slave
 TX    Pin0
 RX    Pin1
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial_slave(0,1); // bluetooth slave
//SoftwareSerial BTSerial_master(2, 3); // bluetooteh master

LiquidCrystal_I2C lcd(0x27,16,2);

#define SS_PIN 10
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'#','0','*'},
  {'9','8','7'},
  {'6','5','4'},
  {'3','2','1'}
};
byte rowPins[rows] = {5, 4, A0, A1}; //connect to the row pinouts of the keypad
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
const int STATUS_NOUSER = 0;
const int STATUS_USERIN = 1;
const int STATUS_CHOICE = 2;
const int STATUS_MONEY = 3;
const int STATUS_WAIT = 4;
String msgBLE;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();        
  for (byte i = 0; i < 6; i++) {
      key.keyByte[i] = 0xFF;
  }
  BTSerial_slave.begin(9600);
//  BTSerial_master.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Device is ready");
  game_status = STATUS_NOUSER;
}

void loop() {
  mfrc522.PCD_Init();
      
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    lcd.setCursor(0,0);
    lcd.print(format_string("Did't detect card"));
    lcd.setCursor(0,1);
    lcd.print(format_string(""));
    delay(300);
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(300);
    return;
  }
  
  if(game_status == 0){
    username = "";
    readBlock(block, readbackblock);
    for(int i=9; i<15; i++){
      if(readbackblock[i] != 0){
        char c = char(readbackblock[i]);
        username += c;
      }
    }
    game_status = STATUS_USERIN;
  }
  
  char key_input = keypad.getKey();
    
  if(game_status == STATUS_USERIN){
    String welcome = format_string("Welcome "+username);
    lcd.setCursor(0,0);
    lcd.print(welcome);
    pre_money = readbackblock[15];
    String money = format_money(pre_money*500);
    lcd.setCursor(0,1);
    lcd.print(money);
    if(key_input == '#'){
      key_input = NO_KEY;
      game_status = STATUS_CHOICE;
    }
  }
  
  if(game_status == STATUS_CHOICE){
    lcd.setCursor(0,0);
    lcd.print("Type your choice");
    if(choice == ""){
      lcd.setCursor(0, 1);
      lcd.print(format_string(""));
    }
    if(key_input != NO_KEY){
      switch(key_input){
        case '1':
          choice = "1";
          break;
        case '2':
          choice = "2";
          break;
        case '3':
          choice = "3";
          break;
        case '4':
          choice = "4";
          break;
        case '5':
          choice = "5";
          break;
        case '6':
          choice = "6";
          break;
        case '7':
          choice = "7";
          break;
        case '8':
          choice = "8";
          break;
        case '#':
          if(choice != ""){
            key_input = NO_KEY;
            game_status = STATUS_MONEY;
          }
          break;
        default:
          lcd.setCursor(0,1);
          lcd.print("Wrong choice");
          break;
      }
    }
    if(choice != ""){
      lcd.setCursor(0,1);
      if(choice == "7"){
        lcd.print(format_string("Small"));
      }else if(choice == "8"){
        lcd.print(format_string("Big"));
      }else{
        lcd.print(format_string(choice));
      }
    }
  }
  
  if(game_status == STATUS_MONEY){
    
    lcd.setCursor(0,0);
    lcd.print(format_string("bet money"));
    lcd.setCursor(0,1);
    lcd.print(format_string(bet_money));
    lcd.setCursor(0,1);
    switch(key_input){
      case '1':
        bet_money += "1";
        
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
          game_status = STATUS_WAIT;
          key_input = NO_KEY;
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
    key_input = NO_KEY;
  }
  
  if(game_status == STATUS_WAIT){
    lcd.setCursor(0,0);
    lcd.print(format_string("Wait for result"));
    lcd.setCursor(0,1);
    String info = choice + ":" + bet_money;
    lcd.print(format_string(info));
    
    if(BTSerial_slave.available()){
      char tmpChar = BTSerial_slave.read();
      msgBLE += tmpChar;
      lcd.setCursor(0,0);
      lcd.print(format_string("Result is: " + msgBLE));
      countResult(msgBLE);
    }
    
    if(key_input == '#'){
      game_status = STATUS_NOUSER;
    }
  }
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

String format_money(long m){
  String final = "$";
  String money = String(m);
  int length = money.length();
  if(length > 6){
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

void countResult(String result){
//  BTSerial_master.print(result);
  
  lcd.setCursor(0,0);
  lcd.print(format_string("Result is: " + result));
//  Serial.println(result);
  delay(2000);
  int r = result.toInt();
  int c = choice.toInt();
  long b = bet_money.toInt();
  if(r == c){
    lcd.setCursor(0, 0);
    lcd.print(format_string("Brovo!"));
    lcd.setCursor(0,1);
    long earn = (b / 500) * 3;
    lcd.print(format_string("You earn " + format_money(earn * 500)));
    writeToRfid(pre_money+earn);
  }else if(r <= 3 && choice.toInt() == 7){
    lcd.setCursor(0, 0);
    lcd.print(format_string("Good!"));
    lcd.setCursor(0,1);
    long earn = (b / 500) * 1;
    lcd.print(format_string("You earn " + format_money(earn * 500)));
    writeToRfid(pre_money+earn);
  }else if(r >= 4 && choice.toInt() == 8){
    lcd.setCursor(0, 0);
    lcd.print(format_string("Good!"));
    lcd.setCursor(0,1);
    long earn = (b / 500) * 1;
    lcd.print(format_string("You earn " + format_money(earn * 500)));
    writeToRfid(pre_money+earn);
  }else{
    lcd.setCursor(0, 0);
    lcd.print(format_string("Try again"));
    lcd.setCursor(0,1);
    long earn = (b / 500);
    lcd.print(format_string("You lose " + format_money(earn * 500)));
    writeToRfid(pre_money-earn);
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
  
  bet_money = "";
  choice = "";
  msgBLE = "";
  game_status = STATUS_NOUSER;
}
