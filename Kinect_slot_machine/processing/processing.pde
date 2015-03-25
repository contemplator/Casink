PImage img;
void setup(){
  size(displayWidth, displayHeight);
  img = loadImage("/Users/idlefox/Desktop/test.jpg");  
  img.resize(width, height);
  image(img,0,0);
}

boolean sketchFullScreen() {
  return true;
}
