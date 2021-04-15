#pragma once

#include <Arduino.h>
#include "WiFi.h"
#include "time.h"

int esp_Wconnect(string WiFi_Network, string WiFi_Pass, int Timeout);//Return 1 se conseguimos conectar 0 se não


int esp_Wcheck();//Return 1 se WiFi está connectada, 0 se não

int esp_Wsync_time ();//Return 0 se a WiFi não estiver ligada