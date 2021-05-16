#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "time.h"
#include "app/app.h"
#include "touch.h"



#define CELL_TOUCH_LISTENERS  8

class App_Delayer: public App {
    private:
        RectangleTouchListener button_touch_listeners[CELL_TOUCH_LISTENERS];

        int hours = 0;
        int minutes = 0;
        int seconds = 0;

        int aux_h = 0;
        int aux_m = 0;
        int aux_s = 0;

        int state = 2;   //0->PAUSED : 1->PLAYING : 2->STOPPED

    public:
        App_Delayer();

        void onOpen() override;   

        void onResume() override;  

        void onTick() override;   

        void onTouchTick() override;

        void onClose() override;   
    
    private:
        void draw_arrows(GBatch_t * batch);

        void draw_hours(GBatch_t * batch);

        void draw_minutes(GBatch_t * batch);

        void draw_seconds(GBatch_t * batch);

        void draw_doubledots(GBatch_t * batch);
        
        void draw_stop_button(GBatch_t * batch);

        void draw_play_button(GBatch_t * batch);

        void draw_pause_button(GBatch_t * batch);

        int check_click_button(TouchData data);

        void timeout_beep(note_t note, uint8_t octave);
};
