#pragma once

#include <Arduino.h>

#include "touch.h"
#include "app/app.h"

#define TOUCH_LISTENERS 9

class App_TicTacToe : public App {
    private:
        TouchListener touch_listeners[TOUCH_LISTENERS];

    public:
        App_TicTacToe();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   

};