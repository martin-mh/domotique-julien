function findArduinoCardStatus()
{
	return Status.findOne();
}

Template.WattsUse.onRendered(function()
{
	var carWatts = Watts.find({type: "car"}, {sort: {to: -1}, limit: 20 }).fetch();
	var globalWatts = Watts.find({type: "global"}, {sort: {to: -1}, limit: 20 }).fetch();
	var watts = [];

	for(var i = 0; i < carWatts.length; ++i)
	{
		var object = {};
		object.carWatts = carWatts[i].watts;
		object.globalWatts = globalWatts[i].watts;
		object.time = globalWatts[i].to.getTime();

		watts.push(object);
	}

	Template.WattsUse.chart = new Morris.Line(
	{
		element: 'watts-chart',
		data: [{globalWatts:0,carWatts:0,time:0}],
		xkey: 'time',
		ykeys: ['globalWatts', 'carWatts'],
		labels: ['Global Watts', 'Car Watts']
	});

});

Template.WattsUse.helpers(
{
	line: function()
	{
		var carWatts = Watts.find({type: "car"}, {sort: {to: -1}, limit: 20 }).fetch();
		var globalWatts = Watts.find({type: "global"}, {sort: {to: -1}, limit: 20 }).fetch();
		var watts = [];

		for(var i = 0; i < carWatts.length; ++i)
		{
			var object = {};
			object.carWatts = carWatts[i].watts;
			object.globalWatts = globalWatts[i].watts;
			object.time = globalWatts[i].to.getTime();

			watts.push(object);
		}

		Template.WattsUse.chart.setData(watts);

		return;
	}
});

Template.HeatersStatusEmbeeded.helpers(
{
	heaters: function()
	{
		var heaters = findArduinoCardStatus().heater;
		return _.map(heaters, function(value, key)
		{
			if(key === 'kitchen')
				key = 'Cuisine';
			else if(key === 'firstLivingRoom')
				key = 'Cuisine et sejour';
			else if(key === 'office')
				key = 'Bureau';
			else if(key === 'secondLivingRoom')
				key = 'Salon';
			else if(key === 'firstBedroom')
				key = 'Chambre des parents';
			else if(key === 'secondBedroom')
				key = 'Chambre enfant';
			else if(key === 'water')
				key = 'Chauffe eau';

			return { status: value, name: key };
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