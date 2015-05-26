var gameStatus = "finish"; // finish, running, waiting, stopping, finish...

// 玫瑰的圖檔
var rose1 = "/resources/white.png", 
	rose2 = "/resources/blue.png",
	rose3 = "/resources/red.png";

// 計時器 setInterval
// ***************
var as1, as2, as3, bs1, bs2, bs3, cs1, cs2, cs3, ds1, ds2, ds3, es1, es2, es3;

// ***************
var rad = [[rose1, rose2, rose3],[rose1, rose2, rose3],[rose1, rose2, rose3],[rose1, rose2, rose3],[rose1, rose2, rose3],[rose1, rose2, rose3]];
// each rose status, false = stop, true = running
// ***************
var af1 = false, af2 = false, af3 = false, bf1 = false, bf2 = false, bf3 = false, cf1 = false, cf2 = false, cf3 = false, 
    df1 = false, df2 = false, df3 = false, ef1 = false, ef2 = false, ef3 = false;
// 記錄每一排最後的排列狀況
// ***************
var result1 = [], result2 = [], result3 = [], result4 = [], result5 = [];
var chahge;

$(document).ready(function(){
	// loading the fruit images and position
	$("#a1").css({top: 20, left: 120, position:'absolute', width: 150, height: 120});
	$("#a1").html(getFruit(0));
	$("#a2").css({top: 175, left: 120, position:'absolute', width: 150, height: 120});
	$("#a2").html(getFruit(0));
	$("#a3").css({top: 325, left: 120, position:'absolute', width: 150, height: 120});
	$("#a3").html(getFruit(0));

	$("#b1").css({top: 20, left: 360, position:'absolute', width: 150, height: 120});
	$("#b1").html(getFruit(1));
	$("#b2").css({top: 175, left: 360, position:'absolute', width: 150, height: 120});
	$("#b2").html(getFruit(1));
	$("#b3").css({top: 325, left: 360, position:'absolute', width: 150, height: 120});
	$("#b3").html(getFruit(1));

	$("#c1").css({top: 20, left: 600, position:'absolute', width: 150, height: 120});
	$("#c1").html(getFruit(2));
	$("#c2").css({top: 175, left:600, position:'absolute', width: 150, height: 120});
	$("#c2").html(getFruit(2));
	$("#c3").css({top: 325, left: 600, position:'absolute', width: 150, height: 120});
	$("#c3").html(getFruit(2));
    
	$("#d1").css({top: 20, left: 835, position:'absolute', width: 150, height: 120});
	$("#d1").html(getFruit(3));
	$("#d2").css({top: 175, left:835, position:'absolute', width: 150, height: 120});
	$("#d2").html(getFruit(3));
	$("#d3").css({top: 325, left: 835, position:'absolute', width: 150, height: 120});
	$("#d3").html(getFruit(3));
	
	$("#e1").css({top: 20, left: 1075, position:'absolute', width: 150, height: 120});
	$("#e1").html(getFruit(4));
	$("#e2").css({top: 175, left: 1075, position:'absolute', width: 150, height: 120});
	$("#e2").html(getFruit(4));
	$("#e3").css({top: 325, left: 1075, position:'absolute', width: 150, height: 120});
	$("#e3").html(getFruit(4));
	
	$("#swf_thorn_close").html('<embed class="thorn" base="." height="100%" loop="false" menu="true" play="true" src="resources/thorn.swf" type="application/x-shockwave-flash" width="580" wmode="transparent"></embed>');
	changeRose();
	console.log("gameStatus: " + gameStatus);
});

// Let user can start game by typing "Enter"
document.onkeydown=keyFunction;
function keyFunction() {
	if(event.keyCode == 13){
		startGame();
	}
}

// judge the game status to start or stop game
// and set an mechanism to prevent user stop game immediately after start gmae
// 當使用者拉下拉桿，就會開始遊戲
// 遊戲會自動停止
// 停止後會計算獎勵
function startGame(){
	if(gameStatus == "finish"){ // 遊戲若是等待開始，則會開始遊戲
		clearInterval(chahge);
		$("#swf_win").html('');
		$("#victory").html('');
		$("#lose").html('');
		$("#pull").html('');
		$("#slot_card").html('');
		money -= 1;
		$("#total_content").text(formatNumber(money*500));
		$("#victory").html('');
		
		rollFruit(1); // 開始轉動
		gameStatus = "running";
		console.log("gameStatus: " + gameStatus);
		// alert($("#victory").attr("autostart"));
		// 等待速度達到最大, 2秒後會達到, gameStauts轉成waitting停止指令
		setTimeout(function(){
			gameStatus = "waiting";
			console.log("gameStatus: " + gameStatus);
		}, 2000);
		setTimeout(function(){
			stopFruit();
		}, 5000);
	}else if(gameStatus == "running"){ // 遊戲進行中，無法進行任何動作
		
	}else if(gameStatus == "waiting"){ // 等待使用者結束指令
		// gameStatus = "stopping";
		// console.log("gameStatus: " + gameStatus);
		// rollFruit(0); 
	}else{

	}
}

// append photo to visible view on browser
function getFruit(number){
	var len = rad[number].length;
	var index = Math.floor(Math.random()*len);
	var fruit = rad[number][index];
	var html = "<img src='"+ fruit + "' class='rose'>";
	rad[number].splice(index, 1);
	return html;
}

// 轉動
// ***************
function rollFruit(s){
	$("#scrolling").html('<embed width="0" height="0" src="resources/main_voice.mp3" autostart="true" loop="true"></embed>');

	var v1 = 20; // 初速度，移動的 y 位置
	var timer1 = 10; // 每 10 毫秒動一次
	var timer2 = 9; // 每 9 毫秒動一次
	var timer3 = 10; // 每 10 毫秒動一次
	var timer4 = 9; // 每 9 毫秒動一次
	var timer5 = 10; // 每 10 毫秒動一次
	var floor = 400; // 最低點
	var ceiling = -100; // 最高點

	// 開始轉動
	as1 = setInterval(function(){roll1($("#a1"))}, timer1);
	as2 = setInterval(function(){roll1($("#a2"))}, timer1);
	as3 = setInterval(function(){roll1($("#a3"))}, timer1);
	bs1 = setInterval(function(){roll2($("#b1"))}, timer2);
	bs2 = setInterval(function(){roll2($("#b2"))}, timer2);
	bs3 = setInterval(function(){roll2($("#b3"))}, timer2);
	cs1 = setInterval(function(){roll3($("#c1"))}, timer3);
	cs2 = setInterval(function(){roll3($("#c2"))}, timer3);
	cs3 = setInterval(function(){roll3($("#c3"))}, timer3);
	ds1 = setInterval(function(){roll4($("#d1"))}, timer4);
	ds2 = setInterval(function(){roll4($("#d2"))}, timer4);
	ds3 = setInterval(function(){roll4($("#d3"))}, timer4);
	es1 = setInterval(function(){roll5($("#e1"))}, timer5);
	es2 = setInterval(function(){roll5($("#e2"))}, timer5);
	es3 = setInterval(function(){roll5($("#e3"))}, timer5);
	af1 = true;
	af2 = true;
	af3 = true;
	bf1 = true;
	bf2 = true;
	bf3 = true;
	cf1 = true;
	cf2 = true;
	cf3 = true;
	df1 = true;
	df2 = true;
	df3 = true;
	ef1 = true;
	ef2 = true;
	ef3 = true;

	// 轉動 function
	// 1. 移動 y 座標
	// 2. 如果到了最低點，就會回到最高點
	function roll1(object){
		position_current = object.css('top');
		position_current = parseInt(position_current);
		if(position_current >= floor){
			position_current = ceiling;
		}
		object.css({top:(position_current + v1), left: 120, position:'absolute', width: 150, height: 120});
	}

	function roll2(object){
		position_current = object.css('top');
		position_current = parseInt(position_current);
		if(position_current >= floor){
			position_current = ceiling;
		}
		object.css({top:(position_current + v1), left: 360, position:'absolute', width: 150, height: 120});
	}

	function roll3(object){
		position_current = object.css('top');
		position_current = parseInt(position_current);
		if(position_current >= floor){
			position_current = ceiling;
		}
		object.css({top:(position_current + v1), left: 600, position:'absolute', width: 150, height: 120});
	}
	
	function roll4(object){
		position_current = object.css('top');
		position_current = parseInt(position_current);
		if(position_current >= floor){
			position_current = ceiling;
		}
		object.css({top:(position_current + v1), left: 835, position:'absolute', width: 150, height: 120});
	}
	
	function roll5(object){
		position_current = object.css('top');
		position_current = parseInt(position_current);
		if(position_current >= floor){
			position_current = ceiling;
		}
		object.css({top:(position_current + v1), left: 1075, position:'absolute', width: 150, height: 120});
	}
}

// 停止
function stopFruit(){
	setTimeout(function(){
		$("#scrolling").html('');
	}, 500);
	
	gameStatus = "finish";
	console.log(gameStatus);

	// 取消原本的轉動
	clearInterval(as1);
	clearInterval(as2);
	clearInterval(as3);
	clearInterval(bs1);
	clearInterval(bs2);
	clearInterval(bs3);
	clearInterval(cs1);
	clearInterval(cs2);
	clearInterval(cs3);
	clearInterval(ds1);
	clearInterval(ds2);
	clearInterval(ds3);
	clearInterval(es1);
	clearInterval(es2);
	clearInterval(es3);

	// 取得每個 rose 現在位置，可以工作之後分配最後位置
	a1_y = $("#a1").position().top;
	a2_y = $("#a2").position().top;
	a3_y = $("#a3").position().top;
	b1_y = $("#b1").position().top;
	b2_y = $("#b2").position().top;
	b3_y = $("#b3").position().top;
	c1_y = $("#c1").position().top;
	c2_y = $("#c2").position().top;
	c3_y = $("#c3").position().top;
	d1_y = $("#d1").position().top;
	d2_y = $("#d2").position().top;
	d3_y = $("#d3").position().top;
	e1_y = $("#e1").position().top;
	e2_y = $("#e2").position().top;
	e3_y = $("#e3").position().top;

	// 開始分配每個 rose 位置
	var fruit_a = ["#a1", "#a2", "#a3"]; // 原始的水果順序 a1, a2, a3, a4
	var fruity_ranka = sortFruit([a1_y, a2_y, a3_y]); // 從最上面到最下面的水果順序
	var fruit_ay = [a1_y, a2_y, a3_y]; // 用來重新排列
	var fruit_ranka = getFruitRank(fruit_ay, fruity_ranka, fruit_a); // 顯示重新排列的水果順序
	var stop1Y = getStopY(fruit_ranka, result1);
	console.log(fruit_ranka);
	// console.log(stop1Y);

	var fruit_b = ["#b1", "#b2", "#b3"];
	var fruity_rankb = sortFruit([b1_y, b2_y, b3_y]);
	var fruit_by = [b1_y, b2_y, b3_y];
	var fruit_rankb = getFruitRank(fruit_by, fruity_rankb, fruit_b);
	var stop2Y = getStopY(fruit_rankb, result2);
	console.log(fruit_rankb);
	
	var fruit_c = ["#c1", "#c2", "#c3"];
	var fruity_rankc = sortFruit([c1_y, c2_y, c3_y]);
	var fruit_cy = [c1_y, c2_y, c3_y];
	var fruit_rankc = getFruitRank(fruit_cy, fruity_rankc, fruit_c);
	var stop3Y = getStopY(fruit_rankc, result3);
	console.log(fruit_rankc);
	
	var fruit_d = ["#d1", "#d2", "#d3"];
	var fruity_rankd = sortFruit([d1_y, d2_y, d3_y]);
	var fruit_dy = [d1_y, d2_y, d3_y];
	var fruit_rankd = getFruitRank(fruit_dy, fruity_rankd, fruit_d);
	var stop4Y = getStopY(fruit_rankd, result4);
	console.log(fruit_rankd);
	
	var fruit_e = ["#e1", "#e2", "#e3"];
	var fruity_ranke = sortFruit([e1_y, e2_y, e3_y]);
	var fruit_ey = [e1_y, e2_y, e3_y];
	var fruit_ranke = getFruitRank(fruit_ey, fruity_ranke, fruit_e);
	var stop5Y = getStopY(fruit_ranke, result5);
	console.log(fruit_ranke);

	// 初始化減速轉動的條件
	var a = 0.1;
	var v1 = 10;
	var minV = 5;
	var timer1 = 10;
	var timer2 = 9;
	var timer3 = 10;
	var timer4 = 9;
	var timer5 = 10;
	var floor = 400;
	var ceiling = -100;
	
	// 開始減速轉動
	as1 = setInterval(function(){slowdown1(as1, $("#a1"), 0)}, timer1);
	as2 = setInterval(function(){slowdown1(as2, $("#a2"), 1)}, timer1);
	as3 = setInterval(function(){slowdown1(as3, $("#a3"), 2)}, timer1);
	bs1 = setInterval(function(){slowdown2(bs1, $("#b1"), 0)}, timer2);
	bs2 = setInterval(function(){slowdown2(bs2, $("#b2"), 1)}, timer2);
	bs3 = setInterval(function(){slowdown2(bs3, $("#b3"), 2)}, timer2);
	cs1 = setInterval(function(){slowdown3(cs1, $("#c1"), 0)}, timer3);
	cs2 = setInterval(function(){slowdown3(cs2, $("#c2"), 1)}, timer3);
	cs3 = setInterval(function(){slowdown3(cs3, $("#c3"), 2)}, timer3);
	ds1 = setInterval(function(){slowdown4(ds1, $("#d1"), 0)}, timer4);
	ds2 = setInterval(function(){slowdown4(ds2, $("#d2"), 1)}, timer4);
	ds3 = setInterval(function(){slowdown4(ds3, $("#d3"), 2)}, timer4);
	es1 = setInterval(function(){slowdown5(es1, $("#e1"), 0)}, timer5);
	es2 = setInterval(function(){slowdown5(es2, $("#e2"), 1)}, timer5);
	es3 = setInterval(function(){slowdown5(es3, $("#e3"), 2)}, timer5);

	function slowdown1(timeInterval, object, order){
		current_position = object.position();
		if(current_position.top >= floor){
			current_position.top = ceiling;
		}
		if(v1 > minV){
			v1 -= a;
		}else{
			v1 = minV;
			if(stop1Y[order] == checkPosition(current_position.top)){
				current_position.top = stop1Y[order];
				clearInterval(timeInterval);
				if(order == 0){
					af1 = false;
					checkAllFruitStatus();
				}else if(order == 1){
					af2 = false;
					checkAllFruitStatus();
				}else if(order == 2){
					af3 = false;
					checkAllFruitStatus();
				}
			}
		}
		object.css({top:(current_position.top + v1), left: 120, position:'absolute', width: 150, height: 120}); // for PD
		// object.css({top:(current_position.top + v1), left: 245, position:'absolute', width: 150, height: 120}); // for RD
	}

	function slowdown2(timeInterval, object, order){
		current_position = object.position();
		if(current_position.top >= floor){
			current_position.top = ceiling;
		}
		if(v1 > minV){
			v1 -= a;
		}else{
			v1 = minV;
			if(stop2Y[order] == checkPosition(current_position.top)){
				current_position.top = stop2Y[order];
				clearInterval(timeInterval);
				if(order == 0){
					bf1 = false;
					checkAllFruitStatus();
				}else if(order == 1){
					bf2 = false;
					checkAllFruitStatus();
				}else if(order == 2){
					bf3 = false;
					checkAllFruitStatus();
				}
			}
		}
		object.css({top:(current_position.top + v1), left: 360, position:'absolute', width: 150, height: 120}); // for PD
		// object.css({top:(current_position.top + v1), left: 640, position:'absolute', width: 150, height: 120}); // for RD
	}

	function slowdown3(timeInterval, object, order){
		current_position = object.position();
		if(current_position.top >= floor){
			current_position.top = ceiling;
		}
		if(v1 > minV){
			v1 -= a;
		}else{
			v1 = minV;
			if(stop3Y[order] == checkPosition(current_position.top)){
				current_position.top = stop3Y[order];
				clearInterval(timeInterval);
				if(order == 0){
					cf1 = false;
					checkAllFruitStatus();
				}else if(order == 1){
					cf2 = false;
					checkAllFruitStatus();
				}else if(order == 2){
					cf3 = false;
					checkAllFruitStatus();
				}
			}
		}
		object.css({top:(current_position.top + v1), left: 600, position:'absolute', width: 150, height: 120}); // for PD
		// object.css({top:(current_position.top + v1), left: 1035, position:'absolute', width: 150, height: 120}); // for RD
	}
	
	function slowdown4(timeInterval, object, order){
		current_position = object.position();
		if(current_position.top >= floor){
			current_position.top = ceiling;
		}
		if(v1 > minV){
			v1 -= a;
		}else{
			v1 = minV;
			if(stop4Y[order] == checkPosition(current_position.top)){
				current_position.top = stop4Y[order];
				clearInterval(timeInterval);
				if(order == 0){
					df1 = false;
					checkAllFruitStatus();
				}else if(order == 1){
					df2 = false;
					checkAllFruitStatus();
				}else if(order == 2){
					df3 = false;
					checkAllFruitStatus();
				}
			}
		}
		object.css({top:(current_position.top + v1), left: 835, position:'absolute', width: 150, height: 120}); // for PD
		// object.css({top:(current_position.top + v1), left: 245, position:'absolute', width: 150, height: 120}); // for RD
	}	
	function slowdown5(timeInterval, object, order){
		current_position = object.position();
		if(current_position.top >= floor){
			current_position.top = ceiling;
		}
		if(v1 > minV){
			v1 -= a;
		}else{
			v1 = minV;
			if(stop5Y[order] == checkPosition(current_position.top)){
				current_position.top = stop5Y[order];
				clearInterval(timeInterval);
				if(order == 0){
					ef1 = false;
					checkAllFruitStatus();
				}else if(order == 1){
					ef2 = false;
					checkAllFruitStatus();
				}else if(order == 2){
					ef3 = false;
					checkAllFruitStatus();
				}
			}
		}
		object.css({top:(current_position.top + v1), left: 1075, position:'absolute', width: 150, height: 120}); // for PD
		// object.css({top:(current_position.top + v1), left: 245, position:'absolute', width: 150, height: 120}); // for RD
	}
	// 判斷遊戲是否結束了
	function checkAllFruitStatus(){
		if(gameStatus == "finish"){
			if(af1 == false && af2 == false && af3 == false && bf1 == false && bf2 == false && bf3 == false && cf1 == false && cf2 == false && cf3 == false && df1 == false && df2 == false && df3 == false && ef1 == false && ef2 == false && ef3 == false){
				$("#scrolling").html('');
				// 回傳這局遊戲的結果
				resultFruit = getMiddleFruit(fruit_ranka, fruit_rankb, fruit_rankc, fruit_rankd, fruit_ranke);
				gameStatus = "finish";
				console.log("gameStatus:" + gameStatus);
			}
		}
	}
}

// *****************************************************
// 要改成適合的高度判斷
// 判斷現在位置是否和指定位置距離不到 10
function checkPosition(position){
	if(Math.abs(position - 175) <= 10){
		return 175;
	}else if(Math.abs(position - 325) <= 10){
		return 325;
	}else if(Math.abs(position - 20) <= 10){
		return 20;
	}else{
		return 1000;
	}
}

function sortNumber(a,b){
	return a - b;
}

function fixPosition(){
	var fruit = [$("#a1"), $("#a2"), $("#a3"), $("#a4"), $("#b1"), $("#b2"), $("#b3"), $("#b4"), $("#c1"), $("#c2"), $("#c3"), $("#c4"),$("#d1"), $("#d2"), $("#d3"),$("#d4"), $("#e1"), $("#e2"), $("#e3"),$("#e4")];
	for(var i=0; i<fruit.length; i++){
		var position = fruit[i].position().top;
		if(Math.abs(position + 160) <= 10){
			fruit[i].position().top = -160;
		}else if(Math.abs(position - 0) <= 10){
			fruit[i].position().top = 0;
		}else if(Math.abs(position - 160) <= 10){
			fruit[i].position().top = 160;
		}else if(Math.abs(position - 320) <= 10){
			fruit[i].position().top = 320;
		}else{
			// console.log("error");
			// console.log(position);
			Math.abs(10 + 160);
			Math.abs(position - 0);
			Math.abs(position - 160)
			Math.abs(position - 320)
		}
		// console.log(fruit[i].position().top);
	}
}

// 計算結果
// 藉由傳進來的每一排的 rose 排列，得知每一排的第二個 rose
function getMiddleFruit(order1, order2, order3, order4, order5){
	var middle1, middle2, middle3, middle4, middle5;
	middle1 = $(order1[1]).html();
	middle2 = $(order2[1]).html();
	middle3 = $(order3[1]).html();
	middle4 = $(order4[1]).html();
	middle5 = $(order5[1]).html();
	console.log(middle1 + ":" + middle2 + ":" + middle3 + ":" + middle4 + ":" + middle5);
	if(middle1 == middle2 && middle2 == middle3 && middle3 == middle4 && middle4 == middle5){
		money += 21;
		$("#total_content").text(formatNumber(money*500));
		$("#victory").html('<embed id="victory" width="0" height="0" src="resources/victory_voice.mp3" autostart="true"></embed>');
		$("#swf_win").html('<embed class="win_light" base="." height="100%" loop="false" menu="true" play="true" src="resources/win.swf" type="application/x-shockwave-flash" width="100%" wmode="transparent"></embed>')
		$("#title").html("<img src='resources/winner.png' class='title-content' />");
		setTimeout(function(){
			$("#swf_winlight").html('');
		}, 2000);
		iosocket.emit('buttonval',money);
		console.log("Win");
	}else if((middle1 == middle2 && middle2 == middle3 && middle2 == middle3 && middle3 == middle4 && middle3 == middle4) || (middle2 == middle3 && middle2 == middle3 && middle3 == middle4 && middle3 == middle4 && middle4 == middle5)){
		money += 11;
		$("#total_content").text(formatNumber(money*500));
		$("#victory").html('<embed id="victory" width="0" height="0" src="resources/victory_voice.mp3" autostart="true"></embed>');
		$("#swf_win").html('<embed class="win_light" base="." height="100%" loop="false" menu="true" play="true" src="resources/win.swf" type="application/x-shockwave-flash" width="100%" wmode="transparent"></embed>')
		$("#title").html("<img src='resources/winner.png' class='title-content' />");
		setTimeout(function(){
			$("#swf_winlight").html('');
		}, 2000);
		iosocket.emit('buttonval',money);
		console.log("Win");
	}else if(middle1 == middle2 && middle2 == middle3 || middle2 == middle3 && middle3 == middle4 || middle3 == middle4 && middle4 == middle5){
		// Win
		money += 2;
		$("#total_content").text(formatNumber(money*500));
		$("#victory").html('<embed id="victory" width="0" height="0" src="resources/victory_voice.mp3" autostart="true"></embed>');
		$("#swf_win").html('<embed class="win_light" base="." height="100%" loop="false" menu="true" play="true" src="resources/win.swf" type="application/x-shockwave-flash" width="100%" wmode="transparent"></embed>')
		$("#title").html("<img src='resources/winner.png' class='title-content' />");
		iosocket.emit('buttonval',money);
		console.log("Win");
	}else{
		// Lose
		// $("#victory").html('<embed id="victory" width="0" height="0" src="resources/victory_voice.mp3" autostart="true"></embed>');
		$("#lose").html('<embed id="lose" width="0" height="0" src="resources/lose.mp3" autostart="true"></embed>');
		$("#swf_thorn_close").html('<embed class="thorn" base="." height="100%" loop="false" menu="true" play="true" src="resources/thorn.swf" type="application/x-shockwave-flash" width="580" wmode="transparent"></embed>');
		$("#title").html("<img src='resources/playagain.png' class='title-content' />");
		console.log("Lose");
		iosocket.emit('buttonval',money);
	}
	console.log("send money :" + money);
	changeRose();
}

// 排列每個 rose y 座標的高低並回傳 (回傳座標)
// 藉由傳進來的每個 rose 的 y 座標去排列
function sortFruit(arrayFruit_y){
	var newFruit_y = arrayFruit_y.sort(sortNumber);
	return newFruit_y;
}

// 藉由 原始的 y 高低排列、排列後的 y 高低排列、每個 rose 被分配到的 id 來重新整理
// 回傳排列後的 rose 陣列，由高至低
function getFruitRank(oldArray, newArray, nameArray){
	var rank = [];
	for(var j=0; j<newArray.length; j++){
		for(var k=0; k<oldArray.length; k++){
			if(newArray[j] == oldArray[k]){
				rank.push(nameArray[k]);
			}
		}
	}
	return rank;
}

// 根據回傳進來的陣列，依據最高的位置去重新給每個位置被分配到的位置
// 範例：
// 	若是最高位置的是 a1，[a1, a2, a3]被分配到的位置如下
// 	[a1, a2, a3] => [-160, 0, 160, 320]
function getStopY(rankFruit, resultArray){
	switch(rankFruit[0]){
		case "#a1":
			resultArray.push($("#a3").html());
			return [20, 175, 325];
		case "#a2":
			resultArray.push($("#a4").html());
			return [325, 20, 175];
		case "#a3":
			resultArray.push($("#a1").html());
			return [175, 325, 20];
		case "#b1":
			resultArray.push($("#b3").html());
			return [20, 175, 325];
		case "#b2":
			resultArray.push($("#b4").html());
			return [325, 20, 175];
		case "#b3":
			resultArray.push($("#b1").html());
			return [175, 325, 20];
		case "#c1":
			resultArray.push($("#c3").html());
			return [20, 175, 325];
		case "#c2":
			resultArray.push($("#c4").html());
			return [325, 20, 175];
		case "#c3":
			resultArray.push($("#c1").html());
			return [175, 325, 20];
		case "#d1":
			resultArray.push($("#d3").html());
			return [20, 175, 325];
		case "#d2":
			resultArray.push($("#d4").html());
			return [325, 20, 175];
		case "#d3":
			resultArray.push($("#d1").html());
			return [175, 325, 20];
		case "#e1":
			resultArray.push($("#e3").html());
			return [20, 175, 325];
		case "#e2":
			resultArray.push($("#e4").html());
			return [325, 20, 175];
		case "#e3":
			resultArray.push($("#e1").html());
			return [175, 325, 20];
		default:
			return null;
	}
}

function changeRose(){
	chahge = setInterval(function(){
		rad = [[rose1, rose2, rose3],[rose1, rose2, rose3],[rose1, rose2, rose3],[rose1, rose2, rose3],[rose1, rose2, rose3]];
		$("#a1").html(getFruit(0));
		$("#a2").html(getFruit(0));
		$("#a3").html(getFruit(0));
		$("#b1").html(getFruit(1));
		$("#b2").html(getFruit(1));
		$("#b3").html(getFruit(1));
		$("#c1").html(getFruit(2));
		$("#c2").html(getFruit(2));
		$("#c3").html(getFruit(2));
		$("#d1").html(getFruit(3));
		$("#d2").html(getFruit(3));
		$("#d3").html(getFruit(3));
		$("#e1").html(getFruit(4));
		$("#e2").html(getFruit(4));
		$("#e3").html(getFruit(4));
	}, 3000);
}

function formatNumber(number){
	var n = number + "";
    var x = n.split('.');
    var x1 = x[0];
    var x2 = x.length > 1 ? '.' + x[1] : '';
    var rgx = /(\d+)(\d{3})/;
    while (rgx.test(x1)) {
        x1 = x1.replace(rgx, '$1' + ',' + '$2');
    }
    console.log(typeof(x1));
    return x1;
}