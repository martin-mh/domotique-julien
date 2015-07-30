Router.route('/', function () 
{
	this.wait(Meteor.subscribe('arduinoStatus'));
	this.wait(Meteor.subscribe('watts', {}, {sort: {to: -1}, limit: 200 }));

	this.layout('layout');

	if(this.ready())
  		this.render('Home');
  	else
  		this.render('Loading');
});

Router.route('/car', function()
{
	this.layout('layout');

	if(this.ready())
		this.render('Car');
	else
		this.render('Loading');
}, {name: 'Car'});