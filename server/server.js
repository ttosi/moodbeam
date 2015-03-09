var net = require('net'),
	express = require('express'),
	moment = require('moment'),
	stringformat = require('string_format');
	
var app = express(),
	clients = [];

var availbleCommands = [
	'showcolor',
	'setbrightness',
	'alternatecolors',
	'twocolor',
	'flashcolor',
	'showrainbow'
];

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
		var command = parseCommand(request.command);
		
		if(clients[request.user] !== undefined && command !== undefined) {
			writeLog(request.user + ' = ' + command);
			clients[request.user].socket.write(command);
		}
	})
})

var server = app.listen(1338, function () {
  console.log('http server running');
})

net.createServer(function (socket) {
	socket.on('data', function(data) {
		data += '';
		var params = data.split(':');
			
		var client = {
			user: params[0],
			password: params[1],
			socket: socket
		};
		
		if(params[1] !== 'keepalive') {
			clients[client.user] = client;
			writeLog('{0} connected'.format(client.user));
		} else {
			if(!clients[client.user]) {
				clients[client.user] = client;
				writeLog('{0} reconnected'.format(client.user));
			}
		}
	});
}).listen(1337);
console.log('tcp server running');

var parseCommand = function(cmd) {
	var command = '',
		isValid = true;
	
	if(availbleCommands.indexOf(cmd.name) >= 0) {
		var colors = [];
		command = cmd.name + ':';
		
		for(var i in cmd.colors) {
			if(cmd.colors[i].every(function(e) { return e >= 0 && e <= 255; })) {
				colors = colors.concat(cmd.colors[i]);
			} else {
				isValid = false;
			}
		}
		command += colors.join(':');
	} else {
		isValid = false;
	}
	
	return isValid ? command : undefined;
};

var writeLog = function(text) {
	var entry = '[{0}] {1}'.format(
		moment().format('MM/DD/YYYY HH:mm:ss'),
		text
	);
	
	console.log(entry);
};
