#include <ESP8266WiFi.h>
#include <dht11.h>

//WiFi settings
const char* ssid = "EUGEN-SAMS_Network_1";  // SSID 
const char* password = "9216411011";  //password

#define PIN_LAMP1 5
#define PIN_DHT1 4

dht11 DHT11;

float temperature;
float humidity;
bool isLamp1;

WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  delay(100);

  pinMode(PIN_LAMP1, OUTPUT);

  //connect to WiFi
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.config(IPAddress(192, 168, 0, 123), IPAddress(192, 168, 0, 1), 
              IPAddress(255, 255, 255, 0), IPAddress(192, 168, 0, 1));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  server.begin();
  Serial.println("Server started");
  Serial.print("Got IP: "); 
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }
   // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  // Match the request
  handle(req, client);
  client.flush(); 
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += temperature;
  //нужно добавить возврат состояния
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
  
void handle(String req,  WiFiClient client)
{
  if (req.indexOf("/temperature") != -1)
  {
     temperature = getTemperature(PIN_DHT1);
  }
  else if (req.indexOf("/humidity") != -1)
  {
     humidity = getHumidity(PIN_DHT1);
  }
  else if (req.indexOf("/lamp1") != -1)
  {
    touchRelay(PIN_LAMP1, isLamp1);
    isLamp1 = !isLamp1;
  }
  else 
  {
    Serial.println("invalid request");
    client.stop();
    return;
  }
}

void touchRelay(int num, bool state)
{
  if (state)
  {
    digitalWrite(num, 1);
  }
  else
  {
    digitalWrite(num, 0);
  }
  //state = !state;
}

float getTemperature(int num)
{
  int chk = DHT11.read(PIN_DHT1);
  Serial.print("Temperature (C): ");
  Serial.println((float)DHT11.temperature, 2);
  return (float)DHT11.temperature;
}

float getHumidity(int num)
{
  int chk = DHT11.read(PIN_DHT1);
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  return (float)DHT11.humidity;
}
