#ifndef _APP_H
#define _APP_H

#include <Arduino.h>

class App {
    public:
        String name;
        String description;

        bool running = false;

    protected:
        unsigned int priority = 1;
        uint32_t stack_depth = 1024;
        int target_core = 1;

        TaskHandle_t task_handle;

    public:
        App(String name, String description);

        void open();
        bool close();

        virtual void onOpen() 
        { };

        virtual void onTick() 
        { };

        virtual void onClose() 
        { };

        
};

#endif
