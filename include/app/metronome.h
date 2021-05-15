#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "touch.h"
#include "app/app.h"

#define METRONOME_CELL_TOUCH_LISTENERS 4

class App_Metronome : public App {
    private:
        RectangleTouchListener button_touch_listeners[METRONOME_CELL_TOUCH_LISTENERS];
        char bpm_print_buffer[16];
        
        int bpm = 100;
        int compass_type = 4;
        int button_on = 0;

    public:
        App_Metronome();

        void onOpen() override; 

        void onResume() override;   

        void onTick() override;   

        void onTouchTick() override;   

        void onClose() override;   

    private:
        void beep_output(note_t note, uint8_t octave);

        int check_click_button(TouchData data);

        void print_button_up(GBatch_t * batch);
        
        void print_button_down(GBatch_t * batch);

        void print_play_button(GBatch_t * batch);

        void print_pause_button(GBatch_t * batch);
        
        void print_bpm(GBatch_t * batch);

        void print_time_string(GBatch_t * batch);

};