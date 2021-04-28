#pragma once

#include <Arduino.h>

#include "touch.h"
#include "app/app.h"

class App_Metronome : public App {

    public:
        App_Metronome();

        void onOpen() override;   

        void onTick() override;   

        void onTouchTick() override;   

        void onClose() override;   

    private:
        void print_button_up();
        
        void print_button_down();
};