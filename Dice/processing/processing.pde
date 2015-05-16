import processing.serial.*;
import processing.video.*;

Serial port;
int lf = 10;
float bringhtness = 0;

Movie movie1;
Movie movie2;
Movie movie3;

int result = 0;
int status = 0; // 0: nothing, 1: playing
int playing = 0;
float md;
float mt;

void setup(){
  size(displayWidth, displayHeight);
  movie1 = new Movie(this, "/Users/idlefox/Desktop/dice_anime.avi");
  movie1.loop();
  movie2 = new Movie(this, "/Users/idlefox/Desktop/dice_animationver.avi");
  movie2.loop();
  movie3 = new Movie(this, "/Users/idlefox/Casink/Dice/processing/anim_card_flip.mp4");
  movie3.loop();
  port = new Serial(this, "/dev/tty.usbmodem1421", 9600);
  port.bufferUntil('\n');

  // video block
  // setup thread
  
}

void draw(){
//  println(mt + ":" + md);
  
  switch(result){
    case 1:
      if(status == 0 || (status == 1 && playing == 1)){
        movie1.play();
        image(movie1, 0, 0);
        status = 1;
        playing = 1;
        md = movie1.duration();
        mt = movie1.time();
      }
      break;
    case 2:
      if(status == 0 || (status == 1 && playing == 2)){
        movie2.play();
        image(movie2, 0, 0);
        status = 1;
        playing = 2;
        md = movie2.duration();
        mt = movie2.time();
      }
      break;
    case 3:
      if(status == 0 || (status == 1 && playing == 3)){
        movie3.play();
        image(movie3, 0, 0);
        status = 1;
        playing = 3;
        md = movie3.duration();
        mt = movie3.time();
      }
      break;
    default:
      break;
  }
  
  if((mt / md) >= 0.95){
    status = 0;
    playing = 0;
    result = 0;
    md = 0;
    mt = 0;
    port.write('R');
    port.write(result);
    port.write('T');
  }
}

void serialEvent( Serial port){
  int inString = port.read();
//  println(inString);
  if(inString == 83){
    println(inString);
    randResult();
  }else{
    println(inString);
  }
}

void movieEvent(Movie movie){
  movie.read();
}

void randResult(){
  if(status == 0){
    float tmp = random(1, 4);
    result = int(tmp);
  }
}

void mousePressed() {
  if(status == 0){
    float tmp = random(1, 4);
    result = int(tmp);
  }
}
