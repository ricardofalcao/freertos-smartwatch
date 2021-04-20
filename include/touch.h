#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

#define TOUCH_SAMPLE_RATE_MS    100

struct TouchData {
    uint16_t x = 0;
    uint16_t y = 0;
    bool pressed = false;
};

/*

*/


struct RectangleTouchListener {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;

    RectangleTouchListener(int32_t x = 0, int32_t y = 0, int32_t width = 0, int32_t height = 0);

    bool contains(TouchData data);
};

class CircleTouchListener {
    int32_t x;
    int32_t y;
    int32_t radius;

    CircleTouchListener(int32_t x, int32_t y, int32_t radius);

    bool contains(TouchData data);
};

/*

*/

class Touch {
    private:
        QueueHandle_t data_queue;

    public:
        Touch();
        
        void begin();

        void calibrate();

        void onTick();

        TouchData getData();

        TouchData waitData();

};

extern Touch touch;