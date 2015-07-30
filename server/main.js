var Mongoose = require('mongoose');
var colors = require('colors');

Mongoose.connect('mongodb://localhost/domotique');

var PrivateWebServer = require('./private-web-server/main.js');
var PublicWebServer = require('./public-server/main.js');

PrivateWebServer.status.on('ready', function(host, port)
{
	console.log(('Private web server ready on '	+ host + ':' + port).green);
});

PrivateWebServer.status.on('error', function()
{
	console.log('Error on starting private web server'.red);
});

PublicWebServer.status.on('ready', function(host, port)
{
	console.log(('Public web server ready on ' + host + ':' + port).green);
});

PublicWebServer.status.on('error', function()
{
	console.log('Error on starting public web server'.red);
});

PublicWebServer.status.on('changeHeater', function(name, status)
{
	PrivateWebServer.changeHeater(name, status);
});

PublicWebServer.status.on('printAmpersToCar', function(ampers)
{
	PrivateWebServer.printAmpersToCar(ampers);
});

PrivateWebServer.start();
PublicWebServer.start();