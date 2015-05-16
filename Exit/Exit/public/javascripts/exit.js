var iosocket;
var fromArduino = "";
var data = "";

window.onload = function() {
	initSocketIO();
};

$(function(){
	Parse.initialize("GxQAcWbTF3xpE9zAgmjYHtdBAgY6Bg7nvkuFbZnO", "1kFaB7tM2V4kDLuLvDxSmgaY4vmg1iNJFPZvB8mR");
	document.onkeydown = keyFunction;
});

function initSocketIO(){
	iosocket = io.connect();
    iosocket.on('update', function (recievedData) {
		if(fromArduino.indexOf("#") > -1){
			fromArduino += recievedData.fromArduino;
		}else{
			fromArduino = recievedData.fromArduino;
		}
		// console.log(fromArduino);

		if(fromArduino.indexOf("#") != -1 && fromArduino.indexOf("$") != -1){
			data = fromArduino.substring(1, fromArduino.length-1);
			console.log(data);
			changeImage(data);
			updateParse(data);
			fromArduino = "";
		}
		
	});
}

function changeImage(data){
	var money = parseInt(data.split(";")[1]);
	if(money < 20){
		$("#projecter").attr("src", "images/01.png");
	}else if(money >= 20 && money < 30){
		$("#projecter").attr("src", "images/01.png");
	}else{
		$("#projecter").attr("src", "images/01.png");
	}
}

function updateParse(data){
	var datas = data.split(";");
	var id = parseInt(datas[0]);
	var money = parseInt(datas[1]*500);

	var Result = Parse.Object.extend("Result");
	var result = new Result();

	result.set("card_id", id);
	result.set("money", money);

	result.save(null, {
		success: function(card) {
			console.log("save data on Parse success");
		},
		error: function(card, error) {
			alert("Error: " + error.code + " " + error.message);
		}
	});
}

function keyFunction(){
	// alert(event.keyCode);
	if(event.keyCode == 13){
		changeDisplay();
	}
}

function changeDisplay(){
	document.location = "rank";
}