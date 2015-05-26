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
int floor = 500;
int ceiling = 100;

FlowObject left, middle, right;

PVector com = new PVector();                                   
PVector com2d = new PVector();
PVector position_hand = new PVector();
PVector position_shoulder = new PVector();

static long lastDebounceTime;
static final int DEBOUNCE_DELAY = 5000;

int money = 0;
int bet = 1000;
String result = "Casink";
boolean handup = false;
boolean currentHand = false;
boolean handmiddle1 = false;
boolean handmiddle2 = false;
boolean currentlHand = false;
boolean isUser = false;

void setup(){
//  size(displayWidth, displayHeight);
  size(800, 600);
  
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
  context.setMirror(true);
  
  background_img = loadImage("background.png");
  background_img.resize(width, height);
  
  minim = new Minim(this);
  player = minim.loadSnippet("main.mp3");
  
  left = new FlowObject(215);
  middle = new FlowObject(355);
  right = new FlowObject(495);
  smooth();
  textSize(32);
}

void draw(){
//  println(mouseY);
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
  PVector position_middle = new PVector();
  context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_TORSO, position_middle);
//  println(position_middle.x);
  if(position_middle.z > 2000 && position_middle.z < 3000 
      && position_middle.x < 600 && position_middle.x > -500){
    isUser = true;
    drawLight();
    PVector position_shoulder = new PVector();
    context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_LEFT_SHOULDER, position_shoulder);
    PVector position_hand = new PVector();
    context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_LEFT_HAND, position_hand);
    PVector position_lhand = new PVector();
    context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_RIGHT_HAND, position_lhand);
    
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
      if(handmiddle2 == true && (left.isRunning == false && 
        middle.isRunning == false && right.isRunning == false)){
        minusBet();
      }
      handmiddle2 = false;
    }
    
    if(position_hand.x < position_middle.x){
      handmiddle1 = true;
    }else{
      if(handmiddle1 == true && (left.isRunning == false && 
        middle.isRunning == false && right.isRunning == false)){
        addBet();
      }
      handmiddle1 = false;
    }
  }else{
    isUser = false;
  }
  
//  fill(255, 0, 0);
  
//  fill(255, 0, 0);
  
//  fill(255, 0, 0);
//  text(position_middle.x, 10, 450);
//  text(position_hand.x, 10, 330);
//  text(position_lhand.x, 10, 210);
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
      right.isRunning == false){
        
        if(money > 1000){
          left = new FlowObject(215);
          middle = new FlowObject(355);
          right = new FlowObject(495);
          left.isRunning = true;
          middle.isRunning = true;
          right.isRunning = true;
          player.rewind();
          player.play();
        }
    }else{
      left.stop();
      middle.stop();
      right.stop();
      
      while(true){
        if(left.isRunning == false && middle.isRunning == false && right.isRunning == false){
          String[] order1 = left.roses;
          String[] order2 = middle.roses;
          String[] order3 = right.roses;
          
          println("-----Main-----");
          println(order1);
          println(order2);
          println(order3);
          
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
//  isUser = true;
}

void onLostUser(SimpleOpenNI curContext, int userId){
  println("onLostUser - userId: " + userId);
//  isUser = false;
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
    ellipse(75, 55, 65, 65);
    fill(252, 222, 40, 80);
    noStroke();
    ellipse(75, 145, 65, 65);
    fill(0, 255, 0, 80);
    noStroke();
    ellipse(75, 235, 65, 65);
  }else if(isUser && left.isRunning == false && 
    middle.isRunning == false && right.isRunning == false){
    fill(255, 0, 0, 80);
    noStroke();
    ellipse(75, 55, 65, 65);
    fill(252, 222, 40, 255);
    noStroke();
    ellipse(75, 145, 65, 65);
    fill(0, 255, 0, 80);
    noStroke();
    ellipse(75, 235, 65, 65);
  }else if(left.isRunning == true || 
    middle.isRunning== true || right.isRunning == true){
    fill(255, 0, 0, 80);
    noStroke();
    ellipse(75, 55, 65, 65);
    fill(252, 222, 40, 225);
    noStroke();
    ellipse(75, 145, 65, 65);
    fill(0, 255, 0, 225);
    noStroke();
    ellipse(75, 235, 65, 65);
  }
}

void drawMoney(int m, int b){
  int m_length = str(m).length();
  int left = 285-m_length*6;
  int b_length = str(b).length();
  int right = 527-b_length*6;
  fill(0,0,0);
  textSize(24);
  text(m, left, 450);
  fill(0,0,0);
  textSize(24);
  text(b, right, 450);
}

void drawResult(String r){
//  println(mouseX);
//  r = "Test";
  int s_length = r.length();
  int left = 390-s_length*6;
  fill(0,0,0);
  textSize(24);
  text(r, left, 188);
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
