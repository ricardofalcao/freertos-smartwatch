#pragma once

#include <Arduino.h>

#include "touch.h"
#include "app/app.h"

class App_TicTacToe : public App {
    private:
        bool ended = false;
        bool empty = true;

    public:
        App_TicTacToe();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   

    private:
        bool check_winner();

        bool check_click_reset(TouchData data);

};