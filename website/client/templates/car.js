function changeCarAmpers(ampers)
{
	var url = 'http://192.168.0.50:3006/printAmpersToCar';

	HTTP.post(url, {headers:{'ampers':ampers}},
		function(err, result)
		{
			if(err) console.log(err); if(result) console.log(result);
		});
}


Template.ChangeAmperage.events(
{
	'click #validate': function(event)
	{
		var ampers = $('#amper');

		changeCarAmpers(ampers[0].value);
	}
})