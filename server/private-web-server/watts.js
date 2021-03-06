/*
 * This receive the watts impulsion and save them into MongoDB in order to
 * monitor the electric consomation
 */

var WattsHistory = require('../models/wattshistory.js');

var ticks = { global: 0, car: 0};

function route(datas, client)
{
	if(!ticks[datas.type])
	{
		ticks[datas.type] = 0;
	}

	ticks[datas.type] += parseInt(datas.watts);
}

function init()
{
	setInterval(function()
	{
		for(var type in ticks)
		{
			WattsHistory.createEntry(ticks[type], type);
			console.log((ticks[type] + ' for ' + type + ' saved.').blue);
			ticks[type] = 0;
		}
	}, 30000);
}

module.exports = { route, init };