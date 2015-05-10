PImage background_img; // 背景圖片的名稱

int image_width = 180; // 每個玫瑰圖片的寬度
int image_height = 180; // 每個玫瑰圖片的高度
int floor = 800; // 最低的y位置
int ceiling = 0; // 最低的x位置

FlowObject left, middle, right; // 每一排玫瑰都是一個Object，可以簡化程式

// 以上是宣告的變數，在下面的地方才會真的有用

void setup(){
  size(displayWidth, displayHeight); // 建立一個全螢幕的Processing程式
  
  background_img = loadImage("background.png"); // 將背景圖片讀進background_img
  background_img.resize(width, height); // 修改圖片大小去適應全螢幕
  
  left = new FlowObject(240); // 建立左排的玫瑰，距離左邊240的距離
  middle = new FlowObject(630);
  right = new FlowObject(1020);
  
  textSize(32); // 指定字的大小，沒用到
  smooth(); // 沒用到
}

void draw(){
  background(0); // 用黑色背景蓋過上一張Frame(上一個動畫)

  // roll the slot machine 
  left.flow(); // 轉動左排
  left.display(); // 顯示左排
  middle.flow();
  middle.display();
  right.flow();
  right.display();
  image(background_img,0,0); // use the background image to cover the roses
}

//boolean sketchFullScreen() {
//  return true;
//}

// 當滑鼠左鍵點下，要做的事情
void mousePressed(){ 
  // 如果左中右都沒在跑，就去讓他們跑
  // else 是說如果他們都在跑，就讓他們停
  if(left.isRunning == false && middle.isRunning == false &&
      right.isRunning == false ){
    left.isRunning = true;
    middle.isRunning = true;
    right.isRunning = true;
  }else{
    left.stop();
    middle.stop();
    right.stop();
  }
}

