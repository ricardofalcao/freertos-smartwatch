#include "app/drawer.h"

#include "tft.h"
#include "touch.h"
#include "graphics.h"

#define APP_WIDTH       ((TFT_WIDTH - 2*APPS_MARGIN_X - APPS_SPACING_X * (DRAWER_COLUMNS - 1)) / DRAWER_COLUMNS)
#define APP_HEIGHT      APP_WIDTH

App_Drawer::App_Drawer() : App("Drawer", "Shows all installed apps") {
    priority = 3;
    stack_depth = 10240;
}

void App_Drawer::addApp(App * app) {
    apps[apps_length++] = app;
}

void App_Drawer::onOpen() {
    this->setup();
}

void App_Drawer::onTick() {
}

void App_Drawer::onTouchTick() {
    TouchData data = touch.waitData();

    for(uint8_t i = 0; i < APPS_PER_PAGE; i++) {
        if(app_touch_listeners[i].contains(data)) {
            uint8_t start = page * APPS_PER_PAGE;
            uint8_t target = start + i;

            if (apps_length <= target) {
                continue;
            }
            
            App * app = apps[target];
            app->open();
            this->close();
            break;
        }
    }
}

void App_Drawer::onClose() {
}

/*

*/

void App_Drawer::setup() {
    graphics.fillScreen(TFT_WHITE);
    
    uint8_t start = page * APPS_PER_PAGE;
    uint8_t end = min(start + APPS_PER_PAGE - 1, apps_length);

    for(uint8_t i = 0; i < APPS_PER_PAGE; i++) {
        int32_t row = (i / 2);
        int32_t col = i % 2;

        int32_t x = APPS_MARGIN_X + col * (APP_WIDTH + APPS_SPACING_X);
        int32_t y = APPS_MARGIN_Y + row * (APP_HEIGHT + APPS_SPACING_Y);

        app_touch_listeners[i].x = x;
        app_touch_listeners[i].y = y;
        app_touch_listeners[i].width = APP_WIDTH;
        app_touch_listeners[i].height = APP_HEIGHT;
    }

    for(uint8_t i = start; i <= end; i++) {
        App * app = apps[i];
        uint8_t cell_index = i - start;

        int32_t row = (cell_index / 2);
        int32_t col = cell_index % 2;

        int32_t x = APPS_MARGIN_X + col * (APP_WIDTH + APPS_SPACING_X);
        int32_t y = APPS_MARGIN_Y + row * (APP_HEIGHT + APPS_SPACING_Y);

        graphics.fillRectangle(x, y, APP_WIDTH, APP_HEIGHT, app->color);
        graphics.drawString(x + APP_WIDTH / 2, y + APP_HEIGHT + 6, app->name.c_str(), TFT_BLACK, 2, TC_DATUM);
    }
}