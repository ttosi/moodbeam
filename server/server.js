var net = require('net');
var http = require('http');
var util = require('util');
var clients = [];
var url = require("url");
var queryString = require("querystring");

var express = require('express')
var app = express()

app.all('/*', function(req, res, next) {
  res.header('Access-Control-Allow-Origin', '*');
  res.header('Access-Control-Allow-Headers', 'X-Requested-With');
  next();
});

app.post('/', function (req, res) {
	// res.set({
		// 'Content-Type': 'text/plain',
		// 'Content-Length': 5,
		// 'Access-Control-Allow-Origin': '*'
	// });
	
	res.send("hello");

	var body = "";
	req.on('data', function(data) {
		body += data;
	});
	
	req.on('end', function() {
		var cmd = JSON.parse(body);
		//console.dir(cmd);
		console.log(cmd.command.colors[1].g);
		//console.log(cmd.colors[1].g);
	})
})

var server = app.listen(1338, function () {
  console.log("http server running");
})

// http.createServer(function (req, res) {
	// res.writeHead(200, { 'Content-Type': 'text/plain' });
	// res.end('Command sent');
	
	//var requestUrl = url.parse(req.url);
	// var data = queryString.parse(url.parse(req.url).query);
	// console.log(data);
	// console.log(data.username);
	// console.log(data.command['name']);
	// console.log(data.command.color[0].r);
	//var command = parseCommand(
	
	//var cmd = req.url.substring(1).split('/');
	//var user = cmd.shift();
	
	// if(client[user] !== undefined) {
		// console.log("command from: " + user + " = " + cmd.join(':'))
		// clients[user].socket.write(cmd.join(':'));
	// }
// }).listen(1338, 'turningdigital.com');
// console.log("http server running");

net.createServer(function (socket) {
	socket.on('data', function(data) {
		data += "";
		var params = data.split(':');
		
		if(params[0] !== 'keepalive') {
			var client = {
				user: params[0],
				password: params[1],
				socket: socket
			};
			
			clients[params[0]] = client;
			console.log("connection from: " + params[0]);
		} else {
			console.log("keepalive from " + params[1] + " on " + Date());
		}
	});
}).listen(1337);
console.log("tcp server running");

var parseCommand = function(cmd) {
	
};