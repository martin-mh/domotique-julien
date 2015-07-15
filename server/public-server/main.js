var EventEmitter = require('events');
var expressApp = require('express')();

var status = new EventEmitter();

function setHeaterStatus(req, res)
{
	var name = req.params.name;
	var _status = req.params.status;
	_status = _status === 'on' ? true : false;

	res.setHeader('Access-Control-Allow-Origin', '*');
	res.send('ok');
	status.emit('changeHeater', name, _status);
}

function start()
{
	expressApp.get('/setHeaterStatus/:name/:status', setHeaterStatus);

	var server = expressApp.listen(3006, function()
	{
		var host = server.address().address;
		var port = server.address().port;

		status.emit('ready', host, port);
	});
}

module.exports = { start, status };