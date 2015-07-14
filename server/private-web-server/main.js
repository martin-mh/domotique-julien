var EventEmitter = require('events');

var express = require('express');
var expressApp = express();

var status = new EventEmitter();

var Watts = require('./watts.js');

expressApp.get('/watts/addTick/:type', Watts.route);

function start()
{
	var server = expressApp.listen(3005, function()
	{
		var host = server.address().address;
		var port = server.address().port;
		
		Watts.init();

		status.emit('ready', host, port);
	})
}

module.exports = { start, status }