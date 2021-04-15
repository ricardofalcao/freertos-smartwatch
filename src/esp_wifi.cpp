

#include <Arduino.h>
#include <WiFi.h>
#include "time.h"


int esp_Wconnect(String WiFi_Network, String WiFi_Pass, int Timeout){
    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFi_Network.c_str(), WiFi_Pass.c_str());

    if(WiFi.status()==WL_CONNECTED)
        return 1;
    else
        return 0;

}
//Return 1 se conseguimos conectar 0 se não


int esp_Wcheck(){
    if(WiFi.status()==WL_CONNECTED)
        return 1;
    else
        return 0;
}//Return 1 se WiFi está connectada, 0 se não


int esp_Wsync_time(){
    String server = "pool.ntp.org";
    
        configTime(0, 3600, server.c_str());
        return 1;
}