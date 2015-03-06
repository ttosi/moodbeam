var net = require('net');
var http = require('http');
var clients = [];

http.createServer(function (req, res) {
	res.writeHead(200, { 'Content-Type': 'text/plain' });
	res.end('Command sent');
	if(req.url !== '/favicon.ico') {
		var cmd = req.url.substring(1).split('/');
		var user = cmd.shift();
		console.log("command from: " + user + " = " + cmd.join(':'))
		clients[user].socket.write(cmd.join(':'));
	}
}).listen(1338, 'turningdigital.com');
console.log("http server running");

net.createServer(function (socket) {
	socket.on('data', function(data) {
		data += "";
		var params = data.split(':');
		var client = {
			user: params[0],
			password: params[1],
			socket: socket
		};
		
		clients[params[0]] = client;
		console.log("connection from: " + params[0]);
	});
	
}).listen(1337);
console.log("tcp server running");
