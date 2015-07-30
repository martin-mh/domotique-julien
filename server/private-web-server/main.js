var EventEmitter = require('events');
var tcp = require('net');
var Json = require('./../utils/json.js');

var status = new EventEmitter();

var Ping = require('./ping.js'); var pingLatency; // Date
var Watts = require('./watts.js');
var Heater = require('./heater.js');
var Car = require('./car.js');

var client;

function proccessMessage(message, c)
{
	console.log(message);
	var message = Json.parse(message);
	
	if(!message || !message.path)
	{
		c.write('You must pass JSON datas and follow the API.');
		return;
	}

	if(message.path == '/watts/addTick')
	{
		Watts.route(message, c);
	}

	else if(message.path == '/ping')
	{
		Ping.route(message, pingLatency, c);
	}
}

function start()
{
	var server = tcp.createServer(function(c)
	{
		client = c;

		c.setEncoding('utf8');
		c.lastMessage = '';
		c.connected = true;

		sendPing();

		console.log('Client connected!');

		var verifyPing = setInterval(function()
		{
			sendPing();
		}, 30000);

		c.on('close', function()
		{
			clearInterval(verifyPing);
			c.connected = false;
			console.log('Client disconnected!');
		});

		c.on('data', function(data)
		{
			if(data === '\0')
			{
				proccessMessage(c.lastMessage, c);
				c.lastMessage = '';
				return;
			}

			c.lastMessage += data;
		});
	});

	server.listen(3005, function()
	{
		Watts.init();

		var address = server.address().address;
		var port = server.address().port;

		status.emit('ready', address, port);
	});
}

function changeHeater(name, status)
{
	Heater.change(name, status, client);

	setTimeout(function()
	{
		sendPing();
	}, 500);
}

function printAmpersToCar(ampers)
{
	Car.printAmpersToCar(ampers, client);
}

function sendPing()
{
	if(!client.connected)
		return;

	client.write(Json.stringify({ path: '/ping' }));
	pingLatency = Date.now();
}

module.exports = { start, status, changeHeater, printAmpersToCar };