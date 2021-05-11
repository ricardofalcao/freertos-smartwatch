#include "app/status_bar.h"

#include "tft.h"
#include "touch.h"
#include "graphics.h"
#include "pins.h"

#define BAR_HEIGHT  STATUSBAR_HEIGHT

App_Statusbar::App_Statusbar() : App("Statusbar", "Shows informations on the status bar") {
    priority = 2;
    stack_depth = 4096;
    
    viewport = { 0, 0, TFT_WIDTH, BAR_HEIGHT };
}

void App_Statusbar::onOpen() {
    graphics.fillScreen(TFT_BLACK);
}

void App_Statusbar::onTick() {
    graphics.beginBatch();

    time_t now;
    time(&now);

    tm current_time;
    localtime_r(&now, &current_time);

    char timeBuffer[7] = {' '};
    sprintf(&timeBuffer[1], "%02d:%02d", current_time.tm_hour, current_time.tm_min);

    graphics.drawFilledString(tft.width() - 5, BAR_HEIGHT / 2, timeBuffer, TFT_WHITE, TFT_BLACK, 2, MR_DATUM);

    graphics.endBatch();

    vAppDelay(1000 / portTICK_PERIOD_MS);
}

void App_Statusbar::onClose() {
}

/*

*/