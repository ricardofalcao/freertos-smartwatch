#pragma once

#include <Arduino.h>

#include "touch.h"
#include "app/app.h"

class App_Metronome : public App {

    public:
        App_Metronome();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   


};