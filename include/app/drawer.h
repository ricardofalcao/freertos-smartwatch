#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "app/app.h"
#include "touch.h"

#define DRAWER_APPS_MAX_LENGTH  16

#define DRAWER_ROWS         1
#define DRAWER_COLUMNS      2

#define PAGE_ARROW_MARGIN_X 40
#define PAGE_ARROW_MARGIN_Y 50
#define PAGE_ARROW_SIZE     40

#define APPS_PER_PAGE       (DRAWER_ROWS * DRAWER_COLUMNS)

#define APPS_MARGIN_X       60
#define APPS_MARGIN_Y       60

#define APPS_SPACING_X      60
#define APPS_SPACING_Y      60

class App_Drawer : public App {
    private:
        App * apps[DRAWER_APPS_MAX_LENGTH];
        size_t apps_length = 0;

        RectangleTouchListener app_touch_listeners[APPS_PER_PAGE];
        RectangleTouchListener next_page_touch_listener;
        RectangleTouchListener previous_page_touch_listener;

        uint8_t page = 0;


        QueueSetHandle_t queue_set;

        QueueHandle_t app_open_queue;
        SemaphoreHandle_t redraw_signal;

    public:
        App_Drawer();

        void addApp(App * app);

        void onOpen() override;   

        void onResume() override;   

        void onTick() override;   

        void onTouchTick() override;   

        void onClose() override;   

    private:
        void click_home();

        void draw_page(GBatch_t * batch);

        void draw_left_arrow(GBatch_t * batch, int32_t color);

        void draw_right_arrow(GBatch_t * batch, int32_t color);

};
