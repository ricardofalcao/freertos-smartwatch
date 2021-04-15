

#include <Arduino.h>
#include "WiFi.h"
#include "time.h"


int esp_Wconnect(string WiFi_Network, string WiFi_Pass, int Timeout){
    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFi_Network,WiFi_Pass);


    while(WiFi.status() != WL_CONNECTED && Timeout>0 ){
        
        delay(10);
        Timeout=Timeout-10;
    }

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
    string server = "pool.ntp.org";
    

     if(WiFi.status()!=WL_CONNECTED)
        return 0;
    else
        configTime(0,3600 , server);
        return 1;
}