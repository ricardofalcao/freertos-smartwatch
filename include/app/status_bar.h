#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "app/app.h"

class App_Statusbar : public App {
    public:
        App_Statusbar();

        void onOpen() override;   

        void onClose() override;   

        void onTick() override;   

};
