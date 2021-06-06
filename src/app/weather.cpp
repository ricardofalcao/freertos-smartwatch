#include "app/weather.h"
#include "notifications.h"

#include <ArduinoJson.h>

#include <WiFi.h>
#include <HTTPClient.h>

#define WIFI_NETWORK "Vodafone-284C30"
#define WIFI_PASS "tYUREqcuVn"

char server[] = "http://api.openweathermap.org/data/2.5/weather";
char apiKey[] = "880bb3d1762b9b1cf8676249d782588c";
char location[] = "porto,PT";

App_weather::App_weather() : App(MSG_WEATHER_NAME, MSG_WEATHER_DESCRIPTION)
{
    priority = 2;
    stack_depth = 10240;
    disableTouch = true;
}

void App_weather::onOpen()
{
    minimized = true;
}

void App_weather::onTick()
{

    while (WiFi.status() != WL_CONNECTED)
    {
        vAppDelay(5000 / portTICK_PERIOD_MS);
        continue;
    }

    vAppDelay(2000 / portTICK_PERIOD_MS);

    char buffer[256];
    sprintf(buffer, "%s?q=%s&APPID=%s", server, location, apiKey);

    HTTPClient http;
    http.begin(buffer);

    int httpResponseCode = http.GET();

    if (httpResponseCode == 200)
    {
        WiFiClient line = http.getStream();
        //create a json buffer where to store the json data
        StaticJsonDocument<2048> jsonBuffer;
        DeserializationError error = deserializeJson(jsonBuffer, line);
        if (error)
        {
            Serial.println("parseObject() failed");
            http.end();
            vAppDelay(5000 / portTICK_PERIOD_MS);
            return;
        }
        //get the data from the json tree
        int nextWeatherTime0 = jsonBuffer["weather"][0]["id"];
        float nextWeather0 = jsonBuffer["main"]["temp"];
        const char *cityName = jsonBuffer["name"];
        //double nextWeather0 = jsonBuffer["main"]["temp"];

        // Print values.
        Serial.println(nextWeatherTime0);
        Serial.println(nextWeather0);

        char notification[32];
        sprintf(notification, "%s: %.1f ºC", cityName, nextWeather0 - 273.15F);
        notifications.enqueueNotification(notification);

        http.end();
        vAppDelay(30000 / portTICK_PERIOD_MS);
    }
    else
    {
        Serial.printf("Could not access weather server. RC=%d\n", httpResponseCode);
        http.end();
        vAppDelay(5000 / portTICK_PERIOD_MS);
    }
}

void App_weather::onClose()
{
}
