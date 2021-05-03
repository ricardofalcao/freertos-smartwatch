#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "app/app.h"

class App_weather : public App {
    

    public:
        App_weather();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   

};
