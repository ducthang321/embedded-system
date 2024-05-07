#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Wire.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "COFFEE 24 HOURS";  //ENTER YOUR WIFI SETTINGS
const char *password = "66668888";
const int DHTPin = 5;
WiFiServer server(80);

#define DHTPIN 0

// OTHER DEFINITIONS
#define DHTTYPE DHT11

// VARIABLES DECLARATION
DHT dht(DHTPIN, DHTTYPE);

float getTemp()
{
    float temp = dht.readTemperature();
    if (isnan(temp))
    {
        return -1;
    }
    return temp;
}
float getHumi()
{
    float temp = dht.readHumidity();
    if (isnan(temp))
    {
        return -1;
    }
    return temp;
}


void setup()
{
    Serial.begin(115200);
    dht.begin();
}
void loop()
{
    delay(2000);

    // read temperature data from DHT11 sensor
    float temperature = getTemp();

    Serial.println(temperature);
    String postData = "?nhietdo=" + String(temp) + "&doam=" + String(humi);
    String link = "#";
    WiFiClient client;
    http.begin(client, link);
    int httpCode = http.POST(postData);            //Send the request
    String payload = http.getString();    //Get the response payload

    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
   
    http.end();  //Close connection
}
