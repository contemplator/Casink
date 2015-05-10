class FlowObject { 
  PImage img1, img2, img3;
  int velocity;
  int margin_left;
  boolean flowing = false;
  int top1, top2, top3;
  int image_width = 180;
  int image_height = 180;
  String[] imgs = {"white.png", "blue.png", "red.png"};
  boolean isRunning;
   
  FlowObject (int left) {  
    margin_left = left;
    top1 = 110;
    top2 = 380;
    top3 = 650;
    
    int index = int(random(0, imgs.length));
    img1 = loadImage(imgs[index]);
    imgs = remove(imgs, index);
    
    index = int(random(imgs.length));
    img2 = loadImage(imgs[index]);
    imgs = remove(imgs, index);
    
    index = int(random(imgs.length));
    img3 = loadImage(imgs[index]);
    imgs = remove(imgs, index);
    
    img1.resize(image_width, image_height);
    img2.resize(image_width, image_height);
    img3.resize(image_width, image_height);
    isRunning = false;
    velocity = int(random(30, 50));
  }
  
  void flow(){
    if(isRunning){
      top1 += velocity;
      top2 += velocity;
      top3 += velocity;
      if(top1 > floor){
        top1 = ceiling;
      }
      if(top2 > floor){
        top2 = ceiling;
      }
      if(top3 > floor){
        top3 = ceiling;
      }
    }
  }
  
  void slow(){
    while(true){
      if(isRunning){
        top1 += velocity;
        top2 += velocity;
        top3 += velocity;
        if(top1 > floor){
          top1 = ceiling;
        }
        if(top2 > floor){
          top2 = ceiling;
        }
        if(top3 > floor){
          top3 = ceiling;
        }
      }
      
      if(velocity >= 0.01){
        velocity -= 0.01;
      }
      
      if(abs(top1-380) <= 20 || abs(top2-380) <= 20 || abs(top3-380) <= 20){
        result(top1, top2, top3);
        isRunning = false;
        return;
      }
    } 
  }
  
  void display(){
    image(img1, margin_left, top1);
    image(img2, margin_left, top2);
    image(img3, margin_left, top3);
  }
  
  void stop(){
    while(true){
      flow();
      display();
      if(abs(top1-380) <= 20 || abs(top2-380) <= 20 || abs(top3-380) <= 20){
        result(top1, top2, top3);
        isRunning = false;
        return;
      }
    }
  }
  
  void result(int t1, int t2, int t3){
    int[] rank = {t1, t2, t3};
    rank = sort(rank);
    t1 = 110;
    t2 = 380;
    t3 = 650;
    flow();
    display();
  }
  
  String[] remove(String array[], int index) {
    String[] new_array = {};
    for (int i = 0; i < array.length; i++) {
      if(i != index){
        new_array = append(new_array, array[i]);
      }
    }
    return new_array;
  }
}
