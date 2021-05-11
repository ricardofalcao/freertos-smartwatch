#pragma once
#include "sdkconfig.h"

#include <Arduino.h>
#include <TFT_eSPI.h>

#include "graphics.h"

#define TOUCH_SAMPLE_RATE_HZ    50

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

    bool contains(GViewport_t viewport, TouchData data);
};

struct CircleTouchListener {
    int32_t x;
    int32_t y;
    int32_t radius;

    CircleTouchListener(int32_t x, int32_t y, int32_t radius);

    bool contains(GViewport_t viewport, TouchData data);
};

/*

*/

class Touch {
    private:
        TouchData oldData;
        QueueHandle_t data_queue;
        EventGroupHandle_t event_group;

    public:
        Touch();
        
        void begin();

        void calibrate();

        void onTick();

        TouchData get();

        TouchData waitRelease();

        TouchData waitPress();

};

extern Touch touch;