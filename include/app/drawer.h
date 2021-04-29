#pragma once

#include <Arduino.h>

#include "app/app.h"
#include "touch.h"

#define DRAWER_MAX_APPS     16

#define DRAWER_ROWS         2
#define DRAWER_COLUMNS      2

#define APPS_PER_PAGE       DRAWER_ROWS * DRAWER_COLUMNS

#define APPS_MARGIN_X       60
#define APPS_MARGIN_Y       60

#define APPS_SPACING_X      60
#define APPS_SPACING_Y      60

class App_Drawer : public App {
    private:
        App * apps[DRAWER_MAX_APPS];
        int apps_length = 0;

        RectangleTouchListener app_touch_listeners[APPS_PER_PAGE];

        uint8_t page = 0;

        QueueHandle_t app_open_queue;

    public:
        App_Drawer();

        void addApp(App * app);

        void onOpen() override;   

        void onTick() override;   

        void onTouchTick() override;   

        void onClose() override;   

    private:
        void setup();

};
