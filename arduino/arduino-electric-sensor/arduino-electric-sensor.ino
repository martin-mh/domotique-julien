#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xA2, 0x2B };
byte ip[] = { 192, 168, 0, 75 };
byte const server[] = { 192, 168, 0, 50 };
int const serverPort = 3005;

int const carWattsPin = 20;
int const carWattsInterrupt = 3;
int const globalWattsPin = 21;
int const globalWattsInterrupt = 2;

EthernetClient client;

volatile int carWatts = 0;
volatile int globalWatts = 0;

void setup()
{
  Ethernet.begin(mac, ip);

  pinMode(carWattsPin, INPUT_PULLUP);
  pinMode(globalWattsPin, INPUT_PULLUP);
  attachInterrupt(carWattsInterrupt, incrementCarWattsCounter, FALLING);
  attachInterrupt(globalWattsInterrupt, incrementGlobalWattsCounter, FALLING);
}

void addTicks(String type, int ticks)
{
  if(!client.connected())
  {
    if(!client.connect(server, serverPort))
    {
      return;
    }
  }

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["path"] = "/watts/addTick";
  root["type"] = type;
  root["watts"] = ticks;

  root.printTo(client);
}

void loop()
{
  delay(20000);

  if(client.available())
  {
    client.flush();
  }

  int _carWatts = carWatts;
  carWatts -= _carWatts;

  int _globalWatts = globalWatts;
  globalWatts -= _globalWatts;

  if(_carWatts != 0)
  {
    addTicks("car", _carWatts);
  }

  if(_globalWatts != 0)
  {
    addTicks("global", _globalWatts);
  }
}

void incrementCarWattsCounter()
{
  if(digitalRead(carWattsPin) != LOW)
    return;
    
  ++carWatts;
}

void incrementGlobalWattsCounter()
{
  if(digitalRead(globalWattsPin) != LOW)
    return;
    
  ++globalWatts;
}