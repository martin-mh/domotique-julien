/*
 * Arduino software
 * Need the ArduinoJson librairie.
 */

#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0xA2, 0x2B };
byte ip[] = { 192, 168, 0, 75 };
byte const server[] = { 192, 168, 0, 50 };
int const serverPort = 3005;

volatile int const carWattsPin = 20;
int const carWattsInterrupt = 3; // Interrupt id for dp 20 is 3
volatile int const globalWattsPin = 21;
int const globalWattsInterrupt = 2; // Interrupt id for dp 21 is 2

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

void setup()
{
  Ethernet.begin(mac, ip);

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

  client.connect(server, serverPort);
}

void loop()
{
  delay(20);
  verifyConnection();

  verifyNewMessages();
}

void verifyConnection()
{
  if(!client.connected())
  {
    client.stop();
    client.connect(server, serverPort);
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

void proccessMessage(String message)
{
  StaticJsonBuffer<194> jsonBuffer;

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

void changeHeater(JsonObject& root)
{
  bool status = root["status"].as<bool>();
  String heater = root["heater"].asString();
  
  if(heater == "kitchenHeater")
  {
    digitalWrite(kitchenHeaterPin, status);
    kitchenHeater = status;
  }
  else if(heater == "firstLivingRoomHeater")
  {
    digitalWrite(firstLivingRoomHeaterPin, status);
    firstLivingRoomHeater = status;
  }
  else if(heater == "officeHeater")
  {
    digitalWrite(officeHeaterPin, status);
    officeHeater = status;
  }
  else if(heater == "secondLivingRoomHeater")
  {
    digitalWrite(secondLivingRoomHeaterPin, status);
    secondLivingRoomHeater = status;
  }
  else if(heater == "firstBedroomHeater")
  {
    digitalWrite(firstBedroomHeaterPin, status);
    firstBedroomHeater = status;
  }
  else if(heater == "secondBedroomHeater")
  {
    digitalWrite(secondBedroomHeaterPin, status);
    secondBedroomHeater = status;
  }
  else if(heater == "waterHeater")
  {
    digitalWrite(waterHeaterPin, status);
    waterHeater = status;
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

/* -- Interrupts -- */

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

inline void addTicks(const String &type, const int ticks)
{
  if(!client)
    return;

  const int BUFFER_SIZE = JSON_OBJECT_SIZE(3);
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["path"] = "/watts/addTick";
  root["type"] = type.c_str();
  root["watts"] = ticks;

  root.printTo(client);
  client.println('\0');
}


