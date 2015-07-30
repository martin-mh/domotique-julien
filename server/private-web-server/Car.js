function computeXORChecksum(s)
{
	var ck = 0;

	for(var i = 0; i < message.length; ++i)
	{
		var c = message.charCodeAt(i);
		ck ^= c;
	}

	return ck;
}

function printAmpersToCar(ampers, client)
{
	var message;
	message = '$SC ' + ampers;

	var checksum = computeXORChecksum(message);
	message += '^';
	message += checksum;
	message += '\r';

	console.log(message);
}