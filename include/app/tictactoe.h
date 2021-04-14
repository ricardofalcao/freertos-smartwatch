#pragma once

#include <Arduino.h>

#include "app/app.h"

class App_TicTacToe : public App {

    public:
        App_TicTacToe();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   

};