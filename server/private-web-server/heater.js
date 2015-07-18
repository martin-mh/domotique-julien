/*
 * This permit you to change heaters status.
 */

var json = require('../utils/json');

function change(name, status, client)
{
	if(!client.connected)
		return;

	var message = {};

	message.heater = name;
	message.status = status;
	message.path = "/changeHeater";

	client.write(json.stringify(message));
}

module.exports = { change };