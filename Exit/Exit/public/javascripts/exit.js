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
	if(money <= 10){
		$("#projecter").attr("src", "images/01.jpg");
	}else if(money > 10 && money <= 20){
		$("#projecter").attr("src", "images/02.jpg");
	}else if(money > 20 && money <= 40){
		$("#projecter").attr("src", "images/03.jpg");
	}else if(money > 40 && money <= 100){
		$("#projecter").attr("src", "images/03.jpg");
	}else if(money > 100){
		$("#projecter").attr("src", "images/05.jpg");
	}
}

function updateParse(data){
	var datas = data.split(";");
	var id = parseInt(datas[0]);
	var money = parseInt(datas[1]*500);

	var Debit = Parse.Object.extend("Debit");
	var query = new Parse.Query(Debit);
	var total_debit = 0;
	query.equalTo("card_id", parseInt(id));
	query.find({
		success: function(debits) {
			for(var i=0; i<debits.length; i++){
				var object = debits[i];
				total_debit += object.get("money");
			}

			var Card = Parse.Object.extend("Card");
			var query_card = new Parse.Query(Card);
			query_card.equalTo("card_id", parseInt(id));
			query_card.find({
				success: function(cards) {
					var Result = Parse.Object.extend("Result");
					var query_result = new Parse.Query(Result);
					query_result.equalTo("card_id", cards[0]);
					query_result.find({
						success: function(results){
							if(results.length > 0){
								return;
							}

							var result = new Result();
							result.set("card_id", cards[0]);
							result.set("department", cards[0].get("department"));
							result.set("money", money - total_debit - 5000);

							result.save(null, {
								success: function(card) {
									console.log("save data on Parse success");
								},
								error: function(card, error) {
									alert("Error: " + error.code + " " + error.message);
								}
							});
						}, error: function(card, error) {
							alert("Error: " + error.code + " " + error.message);
						}
					});
				},
				error: function(card, error) {
					alert("Error: " + error.code + " " + error.message);
				}
			});

		},
		error: function(error) {
			alert("Error: " + error.code + " " + error.message);
		}
	});
	// var debit = new Debit();


	// var Result = Parse.Object.extend("Result");
	// var result = new Result();

	// result.set("card_id", id);
	// result.set("money", money);

	// result.save(null, {
	// 	success: function(card) {
	// 		console.log("save data on Parse success");
	// 	},
	// 	error: function(card, error) {
	// 		alert("Error: " + error.code + " " + error.message);
	// 	}
	// });
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