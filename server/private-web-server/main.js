var EventEmitter = require('events');
var tcp = require('net');
var Json = require('./../utils/json.js');

var status = new EventEmitter();

var Watts = require('./watts.js');

//expressApp.get('/watts/addTick/:type/:count', Watts.route);

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
}

function start()
{
	var server = tcp.createServer(function(c)
	{
		console.log('Client connected!');

		c.setEncoding('utf8');
		c.lastMessage = '';

		c.on('end', function()
		{
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

	server.on
}

module.exports = { start, status }