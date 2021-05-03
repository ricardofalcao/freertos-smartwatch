#pragma once

#include <Arduino.h>

#include "touch.h"
#include "app/app.h"

class App_Monitor : public App {
    public:
        App_Monitor();

        void onOpen() override;   

        void onTick() override;   

        void onTouchTick() override;   

        void onClose() override;   

    private:
        void fillTasks();
};