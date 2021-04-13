#pragma once

#include <Arduino.h>

#include "app/app.h"

class App_Alert : public App {

    public:
        App_Alert();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   

};
