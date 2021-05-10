#pragma once
#include "sdkconfig.h"

#include <Arduino.h>

/*

*/

class Pins {
    public:
        SemaphoreHandle_t gpio0;

    public:
        Pins();
        
        void begin();

};

extern Pins pins;