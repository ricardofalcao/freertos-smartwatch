#pragma once

#include <Arduino.h>

class App {
    public:
        String name;
        String description;

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

        void open();
        bool close();

        virtual void onOpen() 
        { };

        virtual void onTick() 
        { };

        virtual void onTouchTick() 
        { };

        virtual void onClose() 
        { };

        
};
