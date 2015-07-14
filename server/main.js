var Mongoose = require('mongoose');
var colors = require('colors');

Mongoose.connect('mongodb://localhost/domotique');

var PrivateWebServer = require('./private-web-server/main.js')

PrivateWebServer.status.on('ready', function(host, port)
{
	console.log(('Private web server ready on '	+ host + ':' + port).green);
});

PrivateWebServer.status.on('error', function()
{
	console.log('Error on starting private web server'.red);
});

PrivateWebServer.start();