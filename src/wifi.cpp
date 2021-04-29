#include "wifi.h"

#include <Arduino.h>
#include <WiFi.h>
#include "time.h"

void esp_Wsync_time(){
    String server = "pool.ntp.org";
    
    configTime(0, 3600, server.c_str());
}