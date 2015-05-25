class FlowObject { 
  PImage img1, img2, img3;
  int velocity;
  int margin_left;
  boolean flowing = false;
  int top1, top2, top3;
  int image_width = 90;
  int image_height = 90;
  private String[] imgs = {"white.png", "blue.png", "red.png"};
  public String[] roses = {};
  public boolean isRunning;
   
  FlowObject (int left) {  
    margin_left = left;
    top1 = 140;
    top2 = 270;
    top3 = 400;
    
    int index = int(random(0, imgs.length));
    img1 = loadImage(imgs[index]);
    addRoseRank(imgs[index]);
    imgs = remove(imgs, index);
    
    
    index = int(random(imgs.length));
    img2 = loadImage(imgs[index]);
    addRoseRank(imgs[index]);
    imgs = remove(imgs, index);
    
    index = int(random(imgs.length));
    img3 = loadImage(imgs[index]);
    addRoseRank(imgs[index]);
    imgs = remove(imgs, index);
    
    img1.resize(image_width, image_height);
    img2.resize(image_width, image_height);
    img3.resize(image_width, image_height);
    isRunning = false;
    velocity = int(random(20, 35));
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
      
      if(abs(top1-275) <= 1 || abs(top2-275) <= 1 || abs(top3-275) <= 1){
        result(top1, top2, top3);
        isRunning = false;
        println(top1 + ";" + top2 + ";" + top3);
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
      if(abs(top1-270) <= 20 || abs(top2-270) <= 20 || abs(top3-270) <= 20){
        roses = result(top1, top2, top3);
        isRunning = false;
        return;
      }
    }
  }
  
  String[] result(int t1, int t2, int t3){
//    println(roses); 
    int[] pre_order = {t1, t2, t3};
//    println(rank);
    int[] new_order = sort(pre_order);
    String[] new_roses = {};
    for(int i=0; i<new_order.length; i++){
      for(int j=0; j<pre_order.length; j++){
        if(new_order[i] == pre_order[j]){
          new_roses = append(new_roses, roses[j]);
        }
      }
    }
    
//    println("----Object-----");
//    println(new_roses);
    
    return new_roses;
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
  
  void addRoseRank(String img){
    if(img == "white.png"){
      roses = append(roses, "white");
//      println("white");
//      println(roses);
    }else if(img == "blue.png"){
      roses = append(roses, "blue");
    }else if(img == "red.png"){
      roses = append(roses, "red");
    }
//    println(roses);
  }
}
