import SimpleOpenNI.*;

SimpleOpenNI  context;

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

void setup(){
  size(displayWidth, displayHeight);
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
  
  left = new FlowObject(240);
  middle = new FlowObject(630);
  right = new FlowObject(1020);
  
  textSize(32);
  smooth();
}

void draw(){
  context.update(); // update kinect data
  background(0);
  
  int[] userList = context.getUsers();
  for(int i=0;i<userList.length;i++){
    if(context.isTrackingSkeleton(userList[i])){
//      stroke(userClr[ (userList[i] - 1) % userClr.length ] );
      drawSkeleton(userList[i]);
    }
      
    // draw the center of mass
    if(context.getCoM(userList[i],com)){
      context.convertRealWorldToProjective(com,com2d);
      stroke(100,255,0);
      strokeWeight(1);
      beginShape(LINES);
        vertex(com2d.x,com2d.y - 5);
        vertex(com2d.x,com2d.y + 5);

        vertex(com2d.x - 5,com2d.y);
        vertex(com2d.x + 5,com2d.y);
      endShape();
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
}

void drawSkeleton(int userId){
  PVector position = new PVector();
  context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_LEFT_SHOULDER, position);
  text(position.y, 10, 90);
  PVector position2 = new PVector();
  context.getJointPositionSkeleton(userId, SimpleOpenNI.SKEL_LEFT_HAND, position2);
  text(position2.y, 10, 180);
  if(position2.y > position.y){
    startGame();
  }
}


//boolean sketchFullScreen() {
//  return true;
//}

void mousePressed(){
  startGame();
}

void startGame(){
  if(left.isRunning == false && middle.isRunning == false &&
      right.isRunning == false ){
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
        break;
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


void keyPressed(){
  switch(key){
    case ' ':
      context.setMirror(!context.mirror());
      break;
  }
}
