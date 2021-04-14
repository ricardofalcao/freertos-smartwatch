#pragma once

#include <Arduino.h>

#include "app/app.h"

class App_Clock : public App {
    private:
        uint8_t seconds = 0;

    public:
        App_Clock();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   

};
