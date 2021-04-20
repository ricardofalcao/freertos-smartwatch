#pragma once

#include <Arduino.h>

#include "tft.h"
#include "graphics.h"

#define STATUSBAR_HEIGHT   20

class App {
    public:
        String name;
        String description;
        uint32_t color;

        GViewport_t viewport = {
            .x = 0,
            .y = STATUSBAR_HEIGHT,
            .width = TFT_WIDTH,
            .height = TFT_HEIGHT - STATUSBAR_HEIGHT
        };

        bool minimized = false;
        bool running = false;

    protected:
        unsigned int priority = 3;
        unsigned int touch_priority = 3;

        uint32_t stack_depth = 1024;
        uint32_t touch_stack_depth = 1024;

        int target_core = 1;

        TaskHandle_t task_handle;
        TaskHandle_t touch_task_handle;

    public:
        App(String name, String description);

        void open(bool minimized);

        bool close();

        void startTouchTask();

        /*

        */

        virtual void onOpen() 
        { };

        virtual void onTick() 
        { };

        virtual void onTouchTick() 
        { };

        virtual void onClose() 
        { };

        
};
