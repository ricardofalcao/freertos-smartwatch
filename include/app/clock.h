#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "time.h"
#include "app/app.h"
#include "touch.h"

#define CLOCK_ANALOG    0
#define CLOCK_DIGITAL   1

class App_Clock : public App {
    private:
        struct tm current_time;
        struct tm old_time;

        bool force_refresh = false;
        uint8_t type = CLOCK_ANALOG;

        RectangleTouchListener type_touch_listener;

    public:
        App_Clock();

        void onOpen() override;   

        void onResume() override;  

        void onTick() override;   

        void onTouchTick() override;   

        void onClose() override;   

    private:
        void draw_type_button(GBatch_t * batch);

        void draw_analog_frame(GBatch_t * batch);
        
        void draw_digital_frame(GBatch_t * batch);

        void draw_line(GBatch_t * batch, float angle, float size, uint32_t color);

};
