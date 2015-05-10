window.onload = function() {
	initSocketIO();
};

$(function(){
	Parse.initialize("GxQAcWbTF3xpE9zAgmjYHtdBAgY6Bg7nvkuFbZnO", "1kFaB7tM2V4kDLuLvDxSmgaY4vmg1iNJFPZvB8mR");

	var Card = Parse.Object.extend("Card");
	var query = new Parse.Query(Card);
	query.descending("card_id");
	query.limit(1);
	query.find({
		success: function(results) {
			// console.log(results);
			var max_card = 0;
			if(results.length > 0){
				max_card = results[0].get('card_id');
			}
			$("#card_id").val(max_card + 1);
		},
		error: function(error) {
			console.log("Error: " + error.code + " " + error.message);
		}
	});

	var iosocket;
});

function newUser(){
	var username_val = $("#username").val().trim();
	var id_val = $("#student_id").val().trim();
	var department_val = $("#department").val();
	var card_id = parseInt($("#card_id").val());
	var money = $("#money").val();

	console.log(username_val + ":" + id_val + ":" + department_val + ":" + card_id + ":" + money + ":");
	
	var Card = Parse.Object.extend("Card");
	var card = new Card();
	card.set("username", username_val);
	card.set("student_id", id_val);
	card.set("department", department_val);
	card.set("card_id", card_id);
	
	card.save(null, {
		success: function(card) {
			// Hooray! Let them use the app now.
			alert("資料已經輸入完畢，祝您鴻運當頭");
			location.reload();
		},
		error: function(card, error) {
			// Show the error message somewhere and let the user try again.
			alert("Error: " + error.code + " " + error.message);
		}
	});
}

function sendArduino(){
	var username_val = $("#username").val();
	var money_val = 10;
	var _id = $("#card_id").val();
	var dataString = _id + ";" + username_val + ";" + money_val;
	console.log(dataString);
	iosocket.emit('sendArduino', dataString);
}

function checkForm(){
	event.preventDefault();
	if($("#username").val() != "" && $("#student_id").val() != ""){
		newUser();
		sendArduino();
	}
}

function checkName(){
	var username_val = $("#username").val();
	if(checkVal(username_val) == false){
		$("#username").parent().parent().attr("class", "form-group has-error");
		$("#error_message").text("Username 只可以是英文");
	}else if(username_val.length > 6){
		$("#username").parent().parent().attr("class", "form-group has-error");
		$("#error_message").text("Username 長度不可以超過6");
	}else{
		$("#username").parent().parent().attr("class", "form-group");
		$("#error_message").text("");
	}
}

function checkId(){
	var id_val = $("#student_id").val();
	if(id_val.length == 8){
		var detemine_code = id_val.substring(2,5);
		$("#department").val(department[detemine_code]);
	}else if(id_val.length >= 9){
		var detemine_code = id_val.substring(3,6);
		$("#department").val(department[detemine_code]);
	}else{
		$("#student_id").parent().parent().attr("class", "form-group");
		$("#error_message").text("請確認你的學號長度是否正常");
	}
}

function checkVal(str) {
	var regExp = /^[a-zA-Z]/;
	if (regExp.test(str))
		return true;
	else
		return false;
}

function initSocketIO(){
	iosocket = io.connect();
    iosocket.on('update', function (recievedData) {
		console.log("recievedData" + recievedData.fromArduino);
	});
}