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

int const mainHeaterPin = 22;
int const kitchenHeaterPin = 23;
int const firstLivingRoomHeaterPin = 24;
int const officeHeaterPin = 25;
int const secondLivingRoomHeaterPin = 26;
int const firstBedroomHeaterPin = 27;
int const secondBedroomHeaterPin = 28;
int const waterHeaterPin = 29;

bool mainHeater = false;
bool kitchenHeater = false;
bool firstLivingRoomHeater = false;
bool officeHeater = false;
bool secondLivingRoomHeater = false;
bool firstBedroomHeater = false;
bool secondBedroomHeater = false;
bool waterHeater = false;

EthernetClient client;

/*volatile int carWatts = 0;
volatile int globalWatts = 0;*/

void setup()
{
  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  pinMode(carWattsPin, INPUT_PULLUP);
  pinMode(globalWattsPin, INPUT_PULLUP);
  attachInterrupt(carWattsInterrupt, incrementCarWattsCounter, FALLING);
  attachInterrupt(globalWattsInterrupt, incrementGlobalWattsCounter, FALLING);

  pinMode(mainHeaterPin, OUTPUT);
  pinMode(kitchenHeaterPin, OUTPUT);
  pinMode(firstLivingRoomHeaterPin, OUTPUT);
  pinMode(officeHeaterPin, OUTPUT);
  pinMode(secondLivingRoomHeaterPin, OUTPUT);
  pinMode(firstBedroomHeaterPin, OUTPUT);
  pinMode(secondBedroomHeaterPin, OUTPUT);
  pinMode(waterHeaterPin, OUTPUT);

  digitalWrite(mainHeaterPin, HIGH);

  client.connect(server, serverPort);
}

void addTicks(String type, int ticks)
{
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["path"] = "/watts/addTick";
  root["type"] = type.c_str();
  root["watts"] = ticks;

  root.printTo(client);
  client.println('\0');
}

void verifyConnection()
{
  if(!client.connected())
  {
    client.stop();

    if(!client.connect(server, serverPort))
    {
      return;
    }
  }
}

void verifyNewMessages()
{
  String receivedMessage;

  while(client.available())
  {
    char letter = client.read();
    
    if(letter == '\0')
    {
      break;
    }

    receivedMessage += letter;
  }

  proccessMessage(receivedMessage);
}

void changeHeater(JsonObject& root)
{
  bool status = root["status"].as<bool>();
  const char * heater = root["heater"].asString();
  
  if(String(heater) == "kitchenHeater")
  {
    digitalWrite(kitchenHeaterPin, status);
    kitchenHeater = status;
  }
  else if(String(heater) == "firstLivingRoomHeater")
  {
    digitalWrite(firstLivingRoomHeaterPin, status);
    firstLivingRoomHeater = status;
  }
  else if(String(heater) == "officeHeater")
  {
    digitalWrite(officeHeaterPin, status);
    officeHeater = status;
  }
  else if(String(heater) == "secondLivingRoomHeater")
  {
    digitalWrite(secondLivingRoomHeaterPin, status);
    secondLivingRoomHeater = status;
  }
  else if(String(heater) == "firstBedroomHeater")
  {
    digitalWrite(firstBedroomHeaterPin, status);
    firstBedroomHeater = status;
  }
  else if(String(heater) == "secondBedroomHeater")
  {
    digitalWrite(secondBedroomHeaterPin, status);
    secondBedroomHeater = status;
  }
  else if(String(heater) == "waterHeater")
  {
    digitalWrite(waterHeaterPin, status);
    waterHeater = status;
  }
}

void proccessMessage(String message)
{
  StaticJsonBuffer<200> jsonBuffer;

  char jsonMessage[200];
  message.toCharArray(jsonMessage, 200);
  
  JsonObject& root = jsonBuffer.parseObject(jsonMessage);
  String path = root["path"].asString();

  if(path == "/changeHeater")
  {
    changeHeater(root);
  }
  else if(path == "/ping")
  {
    sendPing();
  }
}

void sendPing()
{
  const int BUFFER_SIZE = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(7);
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& ping = jsonBuffer.createObject();

  ping["path"] = "/ping";
  ping["ready"] = true;

  JsonObject& heaters = ping.createNestedObject("heater");
  heaters["kitchen"] = kitchenHeater;
  heaters["firstLivingRoom"] = firstLivingRoomHeater;
  heaters["office"] = officeHeater;
  heaters["secondLivingRoom"] = secondLivingRoomHeater;
  heaters["firstBedroom"] = firstBedroomHeater;
  heaters["secondBedroom"] = secondBedroomHeater;
  heaters["waterHeater"] = waterHeater;

  ping.printTo(client);
  client.println('\0');
}

/*void verifyWatts()
{
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
}*/

void loop()
{
  delay(20);
  verifyConnection();

  /*verifyWatts();*/
  verifyNewMessages();
}

void incrementCarWattsCounter()
{
  if(digitalRead(carWattsPin) != LOW)
    return;
    
  addTicks("car", 1);
}

void incrementGlobalWattsCounter()
{
  if(digitalRead(globalWattsPin) != LOW)
    return;
    
  addTicks("global", 1);
}