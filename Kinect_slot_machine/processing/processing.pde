import processing.serial.*;
import SimpleOpenNI.*;
import ddf.minim.*;

SimpleOpenNI  context;
Serial port;
Minim minim;
AudioSnippet player;

PImage background_img;

int image_width = 180;
int image_height = 180;
int floor = 800;
int ceiling = 0;

FlowObject left, middle, right;

PVector com = new PVector();                                   
PVector com2d = new PVector();
PVector position_hand = new PVector();
PVector position_shoulder = new PVector();

static long lastDebounceTime;
static final int DEBOUNCE_DELAY = 5000;

int money = 0;
int bet = 500;
String result = "Congraduation";
boolean handup = false;
boolean currentHand = false;
boolean handmiddle1 = false;
boolean handmiddle2 = false;
boolean currentlHand = false;
boolean isUser = false;

void setup(){
  size(displayWidth, displayHeight);
  
  port = new Serial(this, "/dev/tty.usbmodem1411", 9600);
  port.bufferUntil('\n');
//  size(640, 480);
  
  context = new SimpleOpenNI(this);
  if(context.isInit() == false){
     println("Can't init SimpleOpenNI, maybe the camera is not connected!"); 
     exit();
     return;
  }
  
  context.enableDepth();
  context.enableUser();
  context.setMirror(false);
  
  background_img = loadImage("background.png");
  background_img.resize(width, height);
  
  minim = new Minim(this);
  player = minim.loadSnippet("main.mp3");
  
  left = new FlowObject(375);
  middle = new FlowObject(630);
  right = new FlowObject(880);
  smooth();
  textSize(32);
}

void draw(){
  println(mouseX + ":" + mouseY);
  context.update(); // update kinect data
  background(255);
  
  int[] userList = context.getUsers();
  for(int i=0;i<userList.length;i++){
    if(context.isTrackingSkeleton(userList[i])){
      drawSkeleton(userList[i]);
    }
  }
  
  // roll the slot machine 
  left.flow();
  left.display();
  middle.flow();
  middle.display();
  right.flow();
  right.display();
  image(background_img,0,0); // use the background image to cover the roses
  
  // display isUser
  drawLight();
  drawMoney(money, bet);
  drawResult(result);
}

void drawSkeleton(int userId){
  PVector position_shoulder = new PVector();
  context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_RIGHT_SHOULDER, position_shoulder);
//  fill(255, 0, 0);
//  text(position_shoulder.y, 10, 90);
  PVector position_hand = new PVector();
  context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_RIGHT_HAND, position_hand);
//  fill(255, 0, 0);
//  text(position_hand.y, 10,180);
  PVector position_lhand = new PVector();
  context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_LEFT_HAND, position_lhand);
  PVector position_middle = new PVector();
  context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_TORSO, position_middle);
  fill(255, 0, 0);
  
//  fill(255, 0, 0);
  
//  fill(255, 0, 0);
  text(position_middle.x, 10, 450);
  text(position_hand.x, 10, 330);
  text(position_lhand.x, 10, 210);
  
  if(position_hand.y > position_shoulder.y){
    handup = true;
  }else{
    if(handup == true){
      startGame();
    }
    handup = false;
  }
  
  if(position_lhand.x > position_middle.x){
    handmiddle2 = true;
  }else{
    if(handmiddle2 == true){
      minusBet();
    }
    handmiddle2 = false;
  }
  
  if(position_hand.x < position_middle.x){
    handmiddle1 = true;
  }else{
    if(handmiddle1 == true){
      addBet();
    }
    handmiddle1 = false;
  }
  
//  if(position_hand.
}


//boolean sketchFullScreen() {
//  return true;
//}

void mousePressed(){
  startGame();
}

void startGame(){
  long currentTime = millis();
  if((currentTime - lastDebounceTime) > DEBOUNCE_DELAY){
    lastDebounceTime = currentTime;
    
    if(left.isRunning == false && middle.isRunning == false &&
      right.isRunning == false ){
      
        left = new FlowObject(375);
        middle = new FlowObject(630);
        right = new FlowObject(880);
        left.isRunning = true;
        middle.isRunning = true;
        right.isRunning = true;
        player.rewind();
        player.play();
    }else{
      left.stop();
      middle.stop();
      right.stop();
      
      while(true){
        if(left.isRunning == false && middle.isRunning == false && right.isRunning == false){
          String[] order1 = left.roses;
          String[] order2 = middle.roses;
          String[] order3 = right.roses;
          
//          println("-----Main-----");
//          println(order1);
//          println(order2);
//          println(order3);
          
          if(order1[1] == order2[1] && order2[1] == order3[1]){
            println("Win");
            int send_bet = abs(bet/500);
            String send_msg = "W" + send_bet + "E";
            port.write(send_msg);
            result = "Win";
            println(send_msg);
          }else{
            println("lose");
            int send_bet = abs(bet/500);
            String send_msg = "L" + send_bet + "E";
            port.write(send_msg);
            result = "Lose";
            println(send_msg);
          }
          break;
        } 
      }
    }
  }
}

// -----------------------------------------------------------------
// SimpleOpenNI events
void onNewUser(SimpleOpenNI curContext, int userId){
  println("onNewUser - userId: " + userId);
  println("\tstart tracking skeleton");
  
  curContext.startTrackingSkeleton(userId);
  isUser = true;
}

void onLostUser(SimpleOpenNI curContext, int userId){
  println("onLostUser - userId: " + userId);
  isUser = false;
}

void onVisibleUser(SimpleOpenNI curContext, int userId){
  //println("onVisibleUser - userId: " + userId);
}

void serialEvent(Serial port){
  while (port.available() > 0) {
    String inBuffer = port.readString();
    println(inBuffer);   
    if (inBuffer != null) {
      if(inBuffer.indexOf("B") >= 0 && inBuffer.indexOf("E") >= 0){
        int indexE = inBuffer.indexOf("E");
        money = parseInt(inBuffer.substring(1, indexE)) * 500;
        println(money);
      }
    }
    break;
  }
}

void keyPressed(){
  switch(key){
    case ' ':
      context.setMirror(!context.mirror());
      break;
  }
}

void drawLight(){
  if(!isUser){
    fill(255, 0, 0, 225);
    noStroke();
    ellipse(115, 95, 115, 115);
    fill(252, 222, 40, 80);
    noStroke();
    ellipse(115, 240, 115, 115);
    fill(0, 255, 0, 80);
    noStroke();
    ellipse(115, 385, 115, 115);
  }else if(isUser && left.isRunning == false && 
    middle.isRunning == false && right.isRunning == false){
    fill(255, 0, 0, 80);
    noStroke();
    ellipse(115, 95, 115, 115);
    fill(252, 222, 40, 255);
    noStroke();
    ellipse(115, 240, 115, 115);
    fill(0, 255, 0, 80);
    noStroke();
    ellipse(115, 385, 115, 115);
  }else if(left.isRunning == true || 
    middle.isRunning== true || right.isRunning == true){
    fill(255, 0, 0, 80);
    noStroke();
    ellipse(115, 95, 115, 115);
    fill(252, 222, 40, 80);
    noStroke();
    ellipse(115, 240, 115, 115);
    fill(0, 255, 0, 225);
    noStroke();
    ellipse(115, 385, 115, 115);
  }
}

void drawMoney(int m, int b){
  fill(0,0,0);
  textSize(32);
  text(m, 465, 670);
  fill(0,0,0);
  textSize(32);
  text(b, 940, 670);
}

void drawResult(String r){
  r = "Try again";
  int s_length = r.length();
  int left = 710-s_length*8;
  fill(0,0,0);
  textSize(32);
  text(r, left, 279);
}

void addBet(){
  if(bet <= (money-500)){
    bet += 500;
  }
}

void minusBet(){
  if(bet > 500){
    bet -= 500;
  }
}
