#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

struct TouchData {
    uint16_t x = 0;
    uint16_t y = 0;
    bool pressed = false;
};

class Touch {
    private:
        QueueHandle_t data_queue;

    public:
        Touch();
        
        void begin();

        void calibrate();

        void onTick();

        TouchData getData();

};

extern Touch touch;