#include "app/weather.h"
 
#include <ArduinoJson.h> 

#include <WiFi.h> 
#include <HTTPClient.h>

#define WIFI_NETWORK "Vodafone-284C30"
#define WIFI_PASS "tYUREqcuVn"

char server[] = "http://api.openweathermap.org/data/2.5/weather";
char apiKey[] = "880bb3d1762b9b1cf8676249d782588c";
char location[] = "porto,PT";

void getWeather()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    continue;
  }

  Serial.println("\nStarting connection to server...");

  char buffer[256];
  sprintf(buffer, "%s?q=%s&APPID=%s", server, location, apiKey);

  HTTPClient http;
  http.begin(buffer);

  int httpResponseCode = http.GET();

  if (httpResponseCode == 200)
  {
    WiFiClient line = http.getStream();
    //create a json buffer where to store the json data
    StaticJsonDocument<1024> jsonBuffer;
    DeserializationError error = deserializeJson(jsonBuffer, line);
    if (error)
    {
      Serial.println("parseObject() failed");
      return;
    }
    //get the data from the json tree
    int nextWeatherTime0 = jsonBuffer["weather"][0]["id"];
    float nextWeather0 = jsonBuffer["main"]["temp"];
    //double nextWeather0 = jsonBuffer["main"]["temp"];

    // Print values.
    Serial.println(nextWeatherTime0);
    Serial.println(nextWeather0);
  } else {

    Serial.printf("Could not access weather server. RC=%d\n", httpResponseCode);
  }

  http.end();
}

App_weather::App_weather() : App(MSG_WEATHER_NAME, MSG_WEATHER_DESCRIPTION)
{
  priority = 5;
  stack_depth = 10240;
}

void App_weather::onOpen()
{
  getWeather();
}

void App_weather::onClose()
{
}
