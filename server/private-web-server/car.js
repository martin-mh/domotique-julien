var json = require('../utils/json');

function computeXORChecksum(s)
{
	var ck = 0;

	for(var i = 0; i < s.length; ++i)
	{
		var c = s.charCodeAt(i);
		ck ^= c;
	}

	return ck.toString(16);
}

function printAmpersToCar(ampers, client)
{
	var message;
	message = '$SC ' + ampers;

	var checksum = computeXORChecksum(message);
	message += '^';
	message += checksum;
	message += '\r';

	var mtc = {}; // Message To Client

	mtc.path = '/carTerminal/print';
	mtc.content = message;

	client.write(json.stringify(mtc));
}

module.exports = { printAmpersToCar };