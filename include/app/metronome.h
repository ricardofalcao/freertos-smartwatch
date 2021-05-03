#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "touch.h"
#include "app/app.h"

#define CELL_TOUCH_LISTENERS 2
class App_Metronome : public App {
    private:
        RectangleTouchListener button_touch_listeners[CELL_TOUCH_LISTENERS];
        char bpm_print_buffer[16];

    public:
        App_Metronome();

        void onOpen() override;   

        void onTick() override;   

        void onTouchTick() override;   

        void onClose() override;   

    private:
        int check_click_button(TouchData data);

        void print_button_up();
        
        void print_button_down();
        
        void print_bpm();
};