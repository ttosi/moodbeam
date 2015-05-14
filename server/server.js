var net = require('net'),
	express = require('express'),
	moment = require('moment'),
	stringformat = require('string_format');
	
var availbleCommands = {
	'showcolor': 0xF0,
	'twocolor': 0xF1,
	'alternatecolors': 0xF2,
	'flash': 0xF3,
	'fade': 0xF4,
};  

var app = express();
var clients = [];

app.all('/*', function(req, res, next) {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Headers', 'X-Requested-With');
  next();
});

app.post('/', function (req, res) {
	var body = '';
	
	req.on('data', function(data) {
		body += data;
	});
	
	req.on('end', function() {
		var request = JSON.parse(body);
		//console.log(request);
		var command = parseCommand(request.command);
		//console.log(command);
		
		if(clients[request.user] !== undefined && command !== undefined) {
			writeLog(request.user + ' = ' + command);
			clients[request.user].socket.write(command);
		}
		
		res.sendStatus(200);
	});
});

var server = app.listen(1338, function () {
  console.log('http server running');
});

net.createServer(function (socket) {
	socket.on('data', function(data) {
		data += '';
		var params = data.split(':'),
			username = params[0];
		
		if(params[1] === 'heartbeat') {
			if(clients[username]) {
				socket.write('ACK');
			}
		} else {
			var client = {
				username: username,
				password: params[1],
				socket: socket
			};
			
			clients[client.username] = client;
			writeLog('{0} connected'.format(client.username));
		}
	});
}).listen(1337, function() {
	console.log('tcp server running');
});

var parseCommand = function(cmd) {
	var command = '',
		isValid = true;
	
	var values = cmd.split(':');
	values[0] = availbleCommands[values[0]];
	
	//values = values.map(function(value) {
	//	return String.fromCharCode(value);
	//});

	command += values.join(':');
	
	//console.log(command);
	
	//if(availbleCommands[cmd] !== undefined) {
	//	var colors;
	//	command = availbleCommands[cmd.name];
		
	//	for(var i in cmd.colors) {
			//colors.push(String.fromCharCode(cmd.colors[i]));
	//		colors += String.fromCharCode(cmd.colors[i]);
			//if(cmd.colors[i].every(function(e) { return e >= 0 && e <= 255; })) {
			//	var x = String.fromCharCode(cmd.colors[i]);
			//	var y = x.charCodeAt(0);
			//	console.log(y);
			//	colors = colors.concat(String.fromCharCode(cmd.colors[i]));
			//} else {
			//	isValid = false;
			//}
	//	}
	//	command += colors.join(':');
	//} else {
	//	isValid = false;
	//}
	
	return command;
	
	//return isValid ? command : undefined;
};

var writeLog = function(text) {
	var entry = '[{0}] {1}'.format(
		moment().format('MM/DD/YYYY HH:mm:ss'),
		text
	);
	
	console.log(entry);
};
