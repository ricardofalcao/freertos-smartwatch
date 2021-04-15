#pragma once

#include <Arduino.h>

#include "app/app.h"

class App_Clock : public App {
    private:
        tm current_time;
        tm old_time;

    public:
        App_Clock();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   

};
