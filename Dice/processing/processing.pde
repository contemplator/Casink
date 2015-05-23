import processing.serial.*;
import processing.video.*;

Serial port;
//int lf = 10;
//float bringhtness = 0;

Movie movie1;
Movie movie2;
Movie movie3;
Movie movie4;
Movie movie5;
Movie movie6;

int result = 0;
int status = 0; // 0: nothing, 1: playing
//int playing = 0;
//float md;
//float mt;

void setup(){
  size(displayWidth, displayHeight);
  movie1 = new Movie(this, "dice_anime_one.avi");
  movie1.noLoop();
  movie2 = new Movie(this, "dice_anime_two.avi");
  movie2.noLoop();
  movie3 = new Movie(this, "dice_anime_three.avi");
  movie3.noLoop();
  movie4 = new Movie(this, "dice_anime_four.avi");
  movie4.noLoop();
  movie5 = new Movie(this, "dice_anime_five.avi");
  movie5.noLoop();
  movie6 = new Movie(this, "dice_anime_six.avi");
  movie6.noLoop();

  port = new Serial(this, "/dev/tty.usbmodem1411", 9600);
  port.bufferUntil('\n');
  
  // video block
  // setup thread
  
}

void draw(){
  if(status == 1){
    switch(result){
      case 1:
//        println(result + "playing");
        movie1.play();
        image(movie1, 0, 0, width, height);
        break;
      case 2:
//        println(result + "playing");
        movie2.play();
        image(movie2, 0, 0, width, height);
        break;
      case 3:
//        println(result + "playing");
        movie3.play();
        image(movie3, 0, 0, width, height);
        break;
      case 4:
//        println(result + "playing");
        movie4.play();
        image(movie4, 0, 0, width, height);
        break;
      case 5:
//        println(result + "playing");
        movie5.play();
        image(movie5, 0, 0, width, height);
        break;
      case 6:
//        println(result + "playing");
        movie6.play();
        image(movie6, 0, 0, width, height);
        break;
    }
  }
//  
//  if((mt / md) >= 0.95){
//    status = 0;
//    playing = 0;
//    result = 0;
//    md = 0;
//    mt = 0;
//    port.write('R');
//    port.write(result);
//    port.write('T');
//  }
}

void serialEvent( Serial port){
  while (port.available() > 0) {
    String inBuffer = port.readString();
//    println(inBuffer);
    if (inBuffer != null) {
      if(inBuffer.indexOf("S") >=0){
//        int indexE = inBuffer.indexOf("S");
//        println(inBuffer.substring(0, indexE+1));
//        if(inBuffer.substring(0, indexE) == "S"){
          randResult();
//        println(money);
      }else{
        println(inBuffer);
      }
//      }
    }
    break;
  }
  
//  int inString = port.read();
//  if(inString == 83){
//    println(inString);
//    randResult();
//    port.clear();
//  }else{
//    println(inString);
//  }
}

void movieEvent(Movie movie){
  movie.read();
  
  if(status == 1 && movie.time() >= 9.0f){
    char c = char(48+result);
    port.write('R');
    port.write(c);
    port.write('T');
    status = 0;
    println("hi:" + result);
    port.clear();
  }
}

void randResult(){
  if(status == 0){
    float tmp = random(1, 7);
    result = int(tmp);
    switch(result){
      case 1:
        movie1.jump(0.0f);
        break;
      case 2:
        movie2.jump(0.0f);
        break;
      case 3:
        movie3.jump(0.0f);
        break;
      case 4:
        movie4.jump(0.0f);
        break;
      case 5:
        movie5.jump(0.0f);
        break;
      case 6:
        movie6.jump(0.0f);
        break;
    }
    status = 1;
  }else{
    println("no");
  }
}

void mousePressed() {
  println("mouse");
  randResult();
//  movie1.jump(0.0f);
//  status = 1;
}

//void playMovie(int result){
//  switch(result){
//    case 0:
//      movie1.play();
//      image(movie1, 0, 0);
//      break;
//    case 1:
//      movie2.play();
//      image(movie2, 0, 0);
//      break;
//    case 2:
//      movie3.play();
//      image(movie3, 0, 0);
//      break;
//    case 3:
//      movie4.play();
//      image(movie4, 0, 0);
//      break;
//    case 4:
//      movie5.play();
//      image(movie5, 0, 0);
//      break;
//    case 5:
//      movie6.play();
//      image(movie6, 0, 0);
//      break;
//    default:
//      break;
//  }
//}
