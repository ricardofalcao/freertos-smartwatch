#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

struct TouchData {
    uint16_t x = 0;
    uint16_t y = 0;
    bool pressed = false;
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

        void callListeners(TouchListener * listeners, size_t listeners_length);

};

extern Touch touch;

/*

*/


class TouchListener {
    public:
        bool active;

    public:
        TouchListener();

        virtual bool contains(uint16_t x, uint16_t y)
        { };
};

class RectangleTouchListener : public TouchListener {
    public:
        int32_t x;
        int32_t y;
        int32_t width;
        int32_t height;

    public:
        RectangleTouchListener(int32_t x, int32_t y, int32_t width, int32_t height);

        bool contains(uint16_t x, uint16_t y) override;
};

class CircleTouchListener : public TouchListener {
    public:
        int32_t x;
        int32_t y;
        int32_t radius;

    public:
        CircleTouchListener(int32_t x, int32_t y, int32_t radius);

        bool contains(uint16_t x, uint16_t y) override;
};