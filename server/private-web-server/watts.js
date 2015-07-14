var WattsHistory = require('../models/wattshistory.js');

var ticks = {};

function route(req, res)
{
	if(!ticks[req.params.type])
	{
		ticks[req.params.type] = 0;
	}

	++ticks[req.params.type];
	res.send('' + ticks[req.params.type]);
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