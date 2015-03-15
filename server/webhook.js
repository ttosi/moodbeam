var express = require('express');
var webhook = require('express-ifttt-webhook');

var app = express();

app.set('port', process.env.PORT || 3000);
console.log(app.get('port'));


app.use(webhook(function(json, done) {
	console.log(json);
	console.log(data);
 
	done();
}));

app.get('/', function(req, res) {
  return res.redirect('https://blog.turningdigital.com');
});

var server = app.listen(app.get('port'), function() {
  console.log('Server listening on port', server.address().port);
});