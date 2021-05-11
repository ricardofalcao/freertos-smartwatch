#include "app/status_bar.h"

#include "tft.h"
#include "touch.h"
#include "graphics.h"
#include "pins.h"

#define BAR_HEIGHT  STATUSBAR_HEIGHT
#define APP_VIEWPORT    GViewport_t{ 0, 0, TFT_WIDTH, BAR_HEIGHT }

App_Statusbar::App_Statusbar() : App("Statusbar", "Shows informations on the status bar") {
    priority = 2;
    stack_depth = 4096;
}

void App_Statusbar::onOpen() {
    GBatch_t batch = graphics.beginBatch(APP_VIEWPORT);
    batch.fillScreen(TFT_BLACK);
    graphics.endBatch(&batch);
}

void App_Statusbar::onTick() {
    GBatch_t batch = graphics.beginBatch(APP_VIEWPORT);

    time_t now;
    time(&now);

    tm current_time;
    localtime_r(&now, &current_time);

    char timeBuffer[7] = {' '};
    sprintf(&timeBuffer[1], "%02d:%02d", current_time.tm_hour, current_time.tm_min);

    batch.drawFilledString(tft.width() - 5, BAR_HEIGHT / 2, timeBuffer, TFT_WHITE, TFT_BLACK, 2, MR_DATUM);

    graphics.endBatch(&batch);

    vAppDelay(1000 / portTICK_PERIOD_MS);
}

void App_Statusbar::onClose() {
}

/*

*/