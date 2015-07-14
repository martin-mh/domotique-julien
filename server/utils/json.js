function stringify(o)
{
	try
	{
		return JSON.stringify(o);
	}
	catch(e)
	{
		return false;
	}
}

function parse(s)
{
	try
	{
		return JSON.parse(s);
	}
	catch(e)
	{
		return false;
	}
}

module.exports = { stringify, parse }