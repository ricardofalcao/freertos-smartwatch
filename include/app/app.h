#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "lang/lang.h"
#include "tft.h"
#include "graphics.h"

#define APP_REGISTRY_MAX_LENGTH     16

#define EVENT_MINIMIZE      0x01
#define EVENT_MINIMIZE_RES  0x02
#define EVENT_RESUME        0x04

#define STATUSBAR_HEIGHT   30

#define DEFAULT_VIEWPORT    GViewport_t{ .x = 0, .y = STATUSBAR_HEIGHT, .width = TFT_WIDTH, .height = TFT_HEIGHT - STATUSBAR_HEIGHT}

class App {
    public:
        static App * APPS_REGISTRY[APP_REGISTRY_MAX_LENGTH];
        static size_t APPS_REGISTRY_LENGTH;

        Message_t name;
        Message_t description;
        uint32_t color;

        bool minimized = false;
        bool running = false;
        
        EventGroupHandle_t event_group;
        
        bool disableTouch = false;

    protected:
        bool canMinimize = true;

        unsigned int priority = 3;
        unsigned int touch_priority = 3;

        uint32_t stack_depth = 4096;
        uint32_t touch_stack_depth = 4096;

        int target_core = 1;

        TaskHandle_t task_handle;
        TaskHandle_t touch_task_handle;

    public:
        App(Message_t name, Message_t description);

        void open();

        void minimize();

        void resume();

        bool close();

        void startTouchTask();

        EventBits_t vAppConditionalDelay(const TickType_t xTicksToDelay, EventBits_t additional_bits);

        void vAppDelay(const TickType_t xTicksToDelay);

        /*

        */

        virtual void onOpen() 
        { };

        virtual void onResume() 
        { };

        virtual void onMinimize() 
        { };

        virtual void onTick() 
        { 
            vAppDelay(portMAX_DELAY);
        };

        virtual void onTouchTick() 
        { 
            vTaskDelay(portMAX_DELAY);
        };

        virtual void onClose() 
        { };

        
};
