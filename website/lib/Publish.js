Meteor.publish('arduinoStatus', function()
{
	return Status.find();
});

Meteor.publish('watts', function(options, selector)
{
	return Watts.find(options, selector);
});