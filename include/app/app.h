#ifndef _APP_H
#define _APP_H

#include <Arduino.h>

#define APP_NOTIFICATION_FLAG_CLOSE 0x01

class App {
    public:
        String name;
        String description;
        int priority;
        bool running = false;

    private:
        TaskHandle_t task_handle;

    public:
        App(String name, String description, int priority);

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
