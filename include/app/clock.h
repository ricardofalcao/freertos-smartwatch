#ifndef _APP_CLOCK_H
#define _APP_CLOCK_H

#include <Arduino.h>

#include "app/app.h"

class App_Clock : public App {

    public:
        App_Clock();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   

};

#endif
