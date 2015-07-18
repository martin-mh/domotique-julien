/*
 * This monitor the main Arduino's status and store it status into MongoDB.
 * This operation is also called Ping.
 */

var Status = require('../models/status.js');

var timeout;

function cardErrored()
{
	Status.getEntry(function(err, s)
	{
		s.connected = false;

		s.save();
	});
}

function route(message, pingLatency, client)
{
	if(timeout)
		clearTimeout(timeout);

	timeout = setTimeout(cardErrored, 50000);

	Status.getEntry(function(err, s)
	{
		s.connected = true;
		s.latency = Date.now() - pingLatency;

		s.heater.kitchen = message.heater.kitchen;
		s.heater.firstLivingRoom = message.heater.firstLivingRoom;
		s.heater.office = message.heater.office;
		s.heater.secondLivingRoom = message.heater.secondLivingRoom;
		s.heater.firstBedroom = message.heater.firstBedroom;
		s.heater.secondBedroom = message.heater.secondBedroom;
		s.heater.waterHeater = message.heater.waterHeater;

		s.save();
	});
}

module.exports = { route };