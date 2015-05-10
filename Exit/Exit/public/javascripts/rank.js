$(function(){
	Parse.initialize("GxQAcWbTF3xpE9zAgmjYHtdBAgY6Bg7nvkuFbZnO", "1kFaB7tM2V4kDLuLvDxSmgaY4vmg1iNJFPZvB8mR");
	document.onkeydown = keyFunction;
	getPersonalRank();
	getDepartmentRank();
});

function getPersonalRank(){
	var Result = Parse.Object.extend("Result");
	var query = new Parse.Query(Result);
	query.descending("money");
	query.include("card_id");
	query.find({
		success: function(results) {
			// console.log(results);
			var html = "<caption>Personal Rank</caption>" + "<tr>" +
				"<th>username</td>" + 
				"<th>money</td>" + 
				"<th>department</td>" + "</tr>";

			for(var i=0; i<results.length; i++){
				var card = results[i].get("card_id");
				var money = results[i].get("money");
				var username = card.get("username");
				var department = card.get("department");

				html += "<tr>" + "<td>" + username + "</td>" +
					"<td>" + money + "</td>" + 
					"<td>" + department + "</td>" + "</tr>";
				// console.log(card.get("username"));
				// console.log(i + ":" + card + ":" + money);
			}

			$("#personal_rank").html(html);
		},
		error: function(error) {
			console.log("Error: " + error.code + " " + error.message);
		}
	});
}

function getDepartmentRank(){
	var Result = Parse.Object.extend("Result");
	var query = new Parse.Query(Result);
	var result_objects = {};
	query.find().then(function(results){
		for(var i=0; i<results.length; i++){
			var id = results[i].id;
			var money = results[i].get("money");
			var department = results[i].get("department");
			// console.log(department);
			var card = {
				"department" : department,
				"money" : money
			}
			result_objects[id] = card;
		}
		return result_objects;
	}).then(function(data){
		var departments = [];
		var moneys = [];
		for(var key in data){
			if(departments.indexOf(data[key]["department"]) != -1){
				var index = departments.indexOf(data[key]["department"]);
				moneys[index] += data[key]["money"];
			}else{
				departments.push(data[key]["department"]);
				moneys.push(data[key]["money"]);
			}
			// console.log(key + ":" + data[key]);
		}

		var rank_data = {};
		for(var i=0; i<departments.length; i++){
			rank_data[departments[i]] = moneys[i];
		}

		moneys.sort(function(a, b){return b-a});

		var new_departments = [];
		for(var i=0; i<moneys.length; i++){
			for(var key in rank_data){
				if(rank_data[key] == moneys[i]){
					new_departments.push(key);
				}
			}
		}
		
		return [new_departments, moneys];
	}).then(function(data){
		var html = "<caption>Department Rank</caption>" + "<tr>" +
			"<th>department</td>" + 
			"<th>money</td>" + "</tr>";
		for(var i=0; i<data[0].length; i++){
			html += "<tr>" + "<td>" + data[0][i] + "</td>" +
				"<td>" + data[1][i] + "</td>" + "</tr>";
		}
		$("#department_rank").html(html);
	});
}

function keyFunction(){
	// alert(event.keyCode);
	if(event.keyCode == 13){
		changeDisplay();
	}
}

function changeDisplay(){
	document.location = "index";
}