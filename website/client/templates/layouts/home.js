function findArduinoCardStatus()
{
	return Status.findOne();
}

function getWattsArray()
{
	var carWatts = Watts.find({type: "car"}, {sort: {to: -1}, limit: 50 }).fetch();
	var globalWatts = Watts.find({type: "global"}, {sort: {to: -1}, limit: 50 }).fetch();
	var watts = [];

	for(var i = 0; i < carWatts.length; ++i)
	{
		var object = {};
		object.carWatts = carWatts[i].watts;
		object.globalWatts = globalWatts[i].watts;
		object.time = globalWatts[i].to.getTime();

		watts.push(object);
	}

	return watts;
}

Template.WattsUse.onRendered(function()
{
	Template.WattsUse.chart = new Morris.Line(
	{
		element: 'watts-chart',
		data: getWattsArray(),
		xkey: 'time',
		ykeys: ['globalWatts', 'carWatts'],
		labels: ['Global Watts', 'Car Watts']
	});

});

Tracker.autorun(function()
	{
		if(Template.WattsUse && Template.WattsUse.chart)
			Template.WattsUse.chart.setData(getWattsArray());

		return;
	}
);

function setHeaterStatus(name, status)
{
	var url = 'http://192.168.0.50:3006/setHeaterStatus/';
	url += name;
	if(name != 'waterHeater')
		url += 'Heater';
	url += '/';
	url += status;
	console.log(url);
	HTTP.get(url, function(err, result){if(err) console.log(err); if(result) console.log(result);})
}

Template.HeatersStatusEmbeeded.events(
{
	'click #switch': function(event)
	{
		var node = $(event.currentTarget);
		setHeaterStatus(node.prop('name'), node.attr('_status'));
	}
});

Template.HeatersStatusEmbeeded.helpers(
{
	heaters: function()
	{
		var heaters = findArduinoCardStatus().heater;
		var _key;
		return _.map(heaters, function(value, key)
		{
			if(key === 'kitchen')
				_key = 'Cuisine et sejour';
			else if(key === 'firstLivingRoom')
				_key = 'Salon et sejour';
			else if(key === 'office')
				_key = 'Bureau';
			else if(key === 'secondLivingRoom')
				_key = 'Salon';
			else if(key === 'firstBedroom')
				_key = 'Chambre des parents';
			else if(key === 'secondBedroom')
				_key = 'Chambre enfant';
			else if(key === 'waterHeater')
				_key = 'Chauffe eau';

			return { status: value, name: _key, regularName: key};
		});
	}
});

Template.ArduinoCardStatusEmbeeded.helpers(
{
	arduinoPing: function()
	{
		return findArduinoCardStatus().latency;
	},
	arduinoStatus: function()
	{
		if(findArduinoCardStatus().connected)
		{
			var content = '<span style="color:green;">';
			content += 'The card is connected and working!';
			content += '</span>';

			return content;
		}
		else
		{
			var content = '<span style="color:red;">';
			content += 'The card is disconnected.';
			content += '</span>';

			return content;
		}
	}

});