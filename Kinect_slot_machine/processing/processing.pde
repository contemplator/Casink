import processing.serial.*;
import SimpleOpenNI.*;

SimpleOpenNI  context;
Serial port;

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
String result = "";
boolean handup = false;
boolean currentHand = false;

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
  
  textSize(32);
  
  left = new FlowObject(375);
  middle = new FlowObject(630);
  right = new FlowObject(880);
  smooth();
}

void draw(){
  context.update(); // update kinect data
  background(255);
  
  int[] userList = context.getUsers();
  for(int i=0;i<userList.length;i++){
    if(context.isTrackingSkeleton(userList[i])){
//      stroke(userClr[ (userList[i] - 1) % userClr.length ] );
      drawSkeleton(userList[i]);
    }
      
    // draw the center of mass
//    if(context.getCoM(userList[i],com)){
//      context.convertRealWorldToProjective(com,com2d);
//      stroke(100,255,0);
//      strokeWeight(1);
//      beginShape(LINES);
//        vertex(com2d.x,com2d.y - 5);
//        vertex(com2d.x,com2d.y + 5);
//
//        vertex(com2d.x - 5,com2d.y);
//        vertex(com2d.x + 5,com2d.y);
//      endShape();
//    }
  }
  
  // roll the slot machine 
  left.flow();
  left.display();
  middle.flow();
  middle.display();
  right.flow();
  right.display();
//  image(background_img,0,0); // use the background image to cover the roses
  fill(0,0,0);
  text(money, 400, 650);
  text(result, 600, 300);
}

void drawSkeleton(int userId){
  PVector position_shoulder = new PVector();
  context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_LEFT_SHOULDER, position_shoulder);
  fill(255, 0, 0);
  text(position_shoulder.y, 10, 90);
  PVector position_hand = new PVector();
  context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_LEFT_HAND, position_hand);
  fill(255, 0, 0);
  text(position_hand.y, 10, 180);
  if(position_hand.y > position_shoulder.y){
    handup = true;
  }else{
    if(handup == true){
      startGame();
    }
    handup = false;
  }
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
            port.write('W');
            result = "Win";
          }else{
            println("lose");
            port.write('L');
            result = "Lose";
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
}

void onLostUser(SimpleOpenNI curContext, int userId){
  println("onLostUser - userId: " + userId);
}

void onVisibleUser(SimpleOpenNI curContext, int userId){
  //println("onVisibleUser - userId: " + userId);
}

void serialEvent(Serial port){
  while (port.available() > 0) {
    String inBuffer = port.readString();   
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
