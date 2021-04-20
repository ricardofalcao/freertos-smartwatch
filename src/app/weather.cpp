#include <SPI.h> 
#include <Arduino.h>
#include <ArduinoJson.h> 
#include <WiFi.h> 
//#include <WiFi101.h> 
#include "esp_wifi.h"
#include "app/weather.h"


#define WIFI_NETWORK "Vodafone-284C30"
#define WIFI_PASS "tYUREqcuVn"

char server[] = "api.openweathermap.org"; 
String apiKey= "880bb3d1762b9b1cf8676249d782588c";
 WiFiClient client; 
 String location= "Porto,PT";


void getWeather() { 
 Serial.println("\nStarting connection to server..."); 
 // if you get a connection, report back via serial: 
 if (client.connect(server, 80)) { 
   Serial.println("connected to server"); 
   // Make a HTTP request: 
   client.print("GET /data/2.5/weather?"); 
   client.print("q="+location); 
   client.print("&appid="+apiKey); 
    
   client.println("&units=metric"); 
   client.println("Host: api.openweathermap.org"); 
   client.println("Connection: close"); 
   client.println(); 
 } else { 
   Serial.println("unable to connect"); 
 } 
 vTaskDelay(1000/portTICK_PERIOD_MS); 

 String line = ""; 
 while (client.connected()) { 
   line = client.readStringUntil('\n'); 
   //Serial.println(line); 
   Serial.println("parsingValues"); 
   //create a json buffer where to store the json data 
   StaticJsonDocument<1024>  jsonBuffer; 
  auto error= deserializeJson(jsonBuffer, line);
   if (error) { 
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
 }
} 

App_weather::App_weather() : App("Weather", "Checks weather through API") {
    priority = 5;
    stack_depth = 1024;
}


void App_weather::onOpen() {
  if(!esp_Wcheck()){
      esp_Wconnect(WIFI_NETWORK,WIFI_PASS,0);
  }
   getWeather();

}

void App_weather::onTick() {
   
}

void App_weather::onClose() {
    
}
