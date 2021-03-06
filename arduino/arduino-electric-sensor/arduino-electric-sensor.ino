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

int const carWattsPin = 20;
int const carWattsInterrupt = 3; // Interrupt id for dp 20 is 3
int const globalWattsPin = 21;
int const globalWattsInterrupt = 2; // Interrupt id for dp 21 is 2

volatile int carWatts = 0;
volatile int globalWatts = 0;

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
  Serial2.begin(115200);
  Ethernet.begin(mac, ip);

  pinMode(carWattsPin, INPUT_PULLUP);
  pinMode(globalWattsPin, INPUT_PULLUP);
  attachInterrupt(carWattsInterrupt, incrementCarWattsCounter, FALLING);
  attachInterrupt(globalWattsInterrupt, incrementGlobalWattsCounter, FALLING);

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

  verifyNewMessages();

  verifyWatts();

  verifyCarTerminal();
}

void verifyCarTerminal()
{
  if(!Serial2.available())
    return;

  String message;

  while(Serial2.available())
  {
    char letter = Serial2.read();

    if(letter == 0x0D) // \r
    {
      break;
    }

    message += letter;
  }

  verifyConnection();

  if(!client)
    return;

  client.print(message);
  client.println('\0');
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
  if(!client.available())
    return;

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

  verifyConnection();
  proccessMessage(receivedMessage);

  if(client.available())
    verifyNewMessages();
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
  else if(path == "/carTerminal/print")
  {
    printToCarTerminal(root);
  }
}

void printToCarTerminal(JsonObject& root)
{
  if(root.containsKey("content"))
  {
    Serial2.print(root["content"].asString());
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
  const int BUFFER_SIZE = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(10);
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

  verifyConnection();

  if(!client)
    return;

  ping.printTo(client);
  client.println('\0');
}

void verifyWatts()
{
  noInterrupts();
    const int global = globalWatts; globalWatts = 0;
    const int car = carWatts; carWatts = 0;
  interrupts();

  if(global != 0)
  {
    addTicks("global", global);
  }

  if(car != 0)
  {
    addTicks("car", car);
  }
}

void addTicks(const String &type, const int ticks)
{
  const int BUFFER_SIZE = JSON_OBJECT_SIZE(3);
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["path"] = "/watts/addTick";
  root["type"] = type.c_str();
  root["watts"] = ticks;

  verifyConnection();

  if(!client)
    return;

  root.printTo(client);
  client.println('\0');
}

/* -- Interrupts -- */

void incrementCarWattsCounter()
{
  if(digitalRead(carWattsPin) != LOW)
    return;
    
  carWatts += 1;
}

void incrementGlobalWattsCounter()
{
  if(digitalRead(globalWattsPin) != LOW)
    return;
    
  globalWatts += 1;
}


