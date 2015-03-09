var net = require('net'),
	express = require('express'),
	clients = [];
	
var app = express();

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
		var command = parseCommand(request);
		
		if(clients[request.username] !== undefined && command !== undefined) {
			console.log(request.username + '=' + command);
			clients[request.username].socket.write(command);
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
		
		if(params[0] !== 'keepalive') {
			var client = {
				user: params[0],
				password: params[1],
				socket: socket
			};
			
			clients[params[0]] = client;
			console.log('connection from: ' + params[0]);
		}
	});
}).listen(1337);
console.log('tcp server running');

var parseCommand = function(cmd) {
	var command = cmd.command.name + ':';
	var colors = [];
	
	for(var i in cmd.command.colors) {
		colors = colors.concat(cmd.command.colors[i]);
	}
	
	return command += colors.join(':');
};