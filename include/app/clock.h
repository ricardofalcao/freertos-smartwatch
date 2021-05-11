#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "time.h"
#include "app/app.h"

class App_Clock : public App {
    private:
        tm current_time;
        tm old_time;

    public:
        App_Clock();

        void onOpen() override;   

        void onResume() override;  

        void onTick() override;   

        void onClose() override;   

    private:
        void draw_line(float angle, float size, uint32_t color);

};
