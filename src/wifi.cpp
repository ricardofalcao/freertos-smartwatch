#include "wifi.h"

#include <WiFi.h>
#include "time.h"

void esp_Wsync_time(){
    char * server = "pool.ntp.org";
    
    configTime(0, 3600, server);
}