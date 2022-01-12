#include <ESP8266WiFi.h>
#include <dht11.h>
#include "FastLED.h"

//WiFi settings
const char* ssid = "EUGEN-SAMS_Network_1";  // SSID 
const char* password = "9216411011";  //password

#define PIN_LAMP1 5
#define PIN_DHT1 4
#define PIN_ILLUMINATE1 14
#define LED_NUM 10

CRGB leds[LED_NUM];
dht11 DHT11;

String response = "";

byte counter;
float temperature;
float humidity;
bool isLamp1;
bool isIlluminate1;
bool change = 0;

WiFiServer server(80);

// struct Str {
//   String type;
//   String delim1;
//   byte number;
//   String delim2;
//   String colour;
//   String delim3;
//   byte brightness;
// }

// Str readStr;

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

  //setting of illumination
  FastLED.addLeds<WS2811, PIN_ILLUMINATE1, GRB>(leds, LED_NUM).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(50);
  pinMode(13, OUTPUT);
}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
  // Wait until the client sends some data
  Serial.println("new client");
  int t = 0;
  while(!client.available())
  {
    delay(100);
    t++;
    if (t >= 10)
    {
      Serial.println("Client disonnected by time");
      return;
    }
  }
   // Read the first line of the request

   String req = client.readStringUntil('\r');
   Serial.println(req);

  // Match the request
  handle(req, client);
  client.flush(); 
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += response;
  Serial.println(s);
  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");
  s = "";
  response = "";  
  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
  
void handle(String req, WiFiClient client)
{

  if (req.indexOf("/temperature") != -1)
  {
     temperature = getTemperature(PIN_DHT1);
     response += "@temperature#";
     response.concat(int(temperature));
     return;
  }
  else if (req.indexOf("/humidity") != -1)
  {
     humidity = getHumidity(PIN_DHT1);
     response += "@humidity#";
     response.concat(int(humidity));
     return;
  }
  else if (req.indexOf("/lamp1") != -1)
  {
    touchRelay(PIN_LAMP1, isLamp1);
    response += "@lamp1#";
    if (isLamp1 == 1)
    {
      response += "off";
    }
    else
    {
      response += "on";
    }
    isLamp1 = !isLamp1;
    return;
  }

  else if (req.indexOf("/illuminate1@White") != -1)
  {
    change = 1;
    changeIllumination(isIlluminate1, "White");
    response += "@illuminate1#";
    if (isIlluminate1 == 0)
    {
      response += "on";
    }
    else
    {
      response += "off";
    }
    isIlluminate1 = !isIlluminate1;
    return;
  }
  else if (req.indexOf("/illuminate1@Red") != -1)
  {
    change = 1;
    changeIllumination(isIlluminate1, "Red");
    response += "@illuminate1#";
    if (isIlluminate1 == 0)
    {
      response += "on";
    }
    else
    {
      response += "off";
    }
    isIlluminate1 = !isIlluminate1;
    return;
  }
  
  else if (req.indexOf("/illuminate1@Green") != -1)
  {
    change = 1;
    changeIllumination(isIlluminate1, "Green");
    response += "@illuminate1#";
    if (isIlluminate1 == 0)
    {
      response += "on";
    }
    else
    {
      response += "off";
    }
    isIlluminate1 = !isIlluminate1;
    return;
  }
  else if (req.indexOf("/illuminate1@Yellow") != -1)
  {
    change = 1;
    changeIllumination(isIlluminate1, "Yellow");
    response += "@illuminate1#";
    if (isIlluminate1 == 0)
    {
      response += "on";
    }
    else
    {
      response += "off";
    }
    isIlluminate1 = !isIlluminate1;
    return;
  }
  else if (req.indexOf("/illuminate1@Blue") != -1)
  {
    change = 1;
    changeIllumination(isIlluminate1, "Blue");
    response += "@illuminate1#";
    if (isIlluminate1 == 0)
    {
      response += "on";
    }
    else
    {
      response += "off";
    }
    isIlluminate1 = !isIlluminate1;
    return;
  }
  else if (req.indexOf("/illuminate1@Rainbow") != -1)
  {
    change = 1;
    changeIllumination(isIlluminate1, "Rainbow");
    response += "@illuminate1#";
    if (isIlluminate1 == 0)
    {
      response += "on";
    }
    else
    {
      response += "off";
    }
    isIlluminate1 = !isIlluminate1;
    return;
  }


  else if (req.indexOf("/synclamp1") != -1)
  {
    response += "@lamp1#";
    if (isLamp1 == 1)
    {
      response += "on";
    }
    else
    {
      response += "off";
    }
    return;
  }
  else if (req.indexOf("/syncIllum1") != -1)
  {
    response += "@illuminate1#";
    if (isIlluminate1 == 1)
    {
      response += "on";
    }
    else
    {
      response += "off";
    }
    return;
  }
  else 
  {
    Serial.println("invalid request");
    client.stop();
    return;
  }
}

void changeIllumination(bool state, String colour)
{
  //while (change == 1)
  {
    if (state)
    {
      for (int i = 0; i < LED_NUM; i++) 
      {
        leds[i] = CRGB::Black;
        FastLED.show();
      }
      change = 0;
    }
    else
    {
      if (colour == "White")
      {
        for (int i = 0; i < LED_NUM; i++) 
        {
          leds[i] = CRGB::White;
          FastLED.show();
        }
        change = 0;
      }
      else if (colour == "Red")
      {
        for (int i = 0; i < LED_NUM; i++) 
        {
          leds[i] = CRGB::Red;
          FastLED.show();
        }
        change = 0;
      }
      else if (colour == "Blue")
      {
        for (int i = 0; i < LED_NUM; i++) 
        {
          leds[i] = CRGB::Blue;
          FastLED.show();
        }
        change = 0;
      }
      else if (colour == "Yellow")
      {
        for (int i = 0; i < LED_NUM; i++) 
        {
          leds[i] = CRGB::Yellow;
          FastLED.show();
        }
        change = 0;
      }
      else if (colour == "Green")
      {
        for (int i = 0; i < LED_NUM; i++) 
        {
          leds[i] = CRGB::Green;
          FastLED.show();
        }
        change = 0;
      }
      else if (colour == "Rainbow")
      {
        for (int i = 0; i < LED_NUM; i++ ) {        
          leds[i] = CHSV(counter + i * 2, 255, 255);  
        }
        counter++;        
        FastLED.show();
        delay(5); 
        //change = 0;
      }  
    }
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
