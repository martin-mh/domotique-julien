#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xA2, 0x2B };
byte ip[] = { 192, 168, 0, 75 };
byte const server[] = { 192, 168, 0, 50 };
int const serverPort = 3005;

int const carWattsPin = 21;
int const globalWattsPin = 20;

EthernetClient client;

void setup()
{
	Ethernet.begin(mac, ip);
	Serial.begin(9600);

	pinMode(carWattsPin, INPUT);
	pinMode(globalWattsPin, INPUT);
}

void addTick(String type)
{
	String path = "GET /watts/addTick/" + type + " HTTP/1.1";

	if(client.connect(server, serverPort))
	{
		client.println(path);
		client.println();

		while(client.connected())
		{
			if(client.available())
			{
				char c = client.read();
				Serial.print(c);
			}
		}

		client.stop();
		Serial.println();
	}
}

void loop()
{
	if(digitalRead(carWattsPin) == HIGH)
	{
		addTick("car");
	}

	if(digitalRead(globalWattsPin) == HIGH)
	{
		addTick("global");
	}
}
