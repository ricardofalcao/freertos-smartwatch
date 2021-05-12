#include "app/status_bar.h"

#include "tft.h"
#include "touch.h"
#include "graphics.h"
#include "pins.h"

#include "icons/statusbar/wifi.h"

#include <WiFi.h>

#define BAR_HEIGHT  STATUSBAR_HEIGHT
#define APP_VIEWPORT    GViewport_t{ 0, 0, TFT_WIDTH, BAR_HEIGHT }

#define STATUSBAR_SPACING       8
#define ICON_BATTERY_WIDTH      16
#define ICON_BATTERY_HEIGHT     (BAR_HEIGHT - 8)

App_Statusbar::App_Statusbar() : App("Statusbar", "Shows informations on the status bar") {
    priority = 2;
    stack_depth = 4096;
    minimized = true;

    icon_wifi.setColorDepth(4);
}

void App_Statusbar::onOpen() {
    icon_wifi.createSprite(ICON_STATUSBAR_WIFI_WIDTH, ICON_STATUSBAR_WIFI_HEIGHT);
    icon_wifi.pushImage(0, 0, ICON_STATUSBAR_WIFI_WIDTH, ICON_STATUSBAR_WIFI_HEIGHT, (uint16_t *) icon_statusbar_wifi);
    icon_wifi.createPalette(icon_statusbar_wifi_palette, 16);

    GBatch_t batch = graphics.beginBatch(APP_VIEWPORT);
    batch.fillScreen(TFT_BLACK);
    graphics.endBatch(&batch);
}

void App_Statusbar::onTick() {
    GBatch_t batch = graphics.beginBatch(APP_VIEWPORT);

    time_t now;
    time(&now);

    struct tm current_time;
    localtime_r(&now, &current_time);

    char timeBuffer[8];
    sprintf(timeBuffer, "%02d:%02d  ", current_time.tm_hour, current_time.tm_min);

    batch.drawFilledString(STATUSBAR_SPACING, BAR_HEIGHT / 2, timeBuffer, TFT_WHITE, TFT_BLACK, 2, ML_DATUM);

    int32_t x = tft.width() - STATUSBAR_SPACING;

    x -= ICON_BATTERY_WIDTH;

    float battery = 0.6;
    uint32_t batteryColor = battery > 0.8 ? TFT_GREEN : battery > 0.5 ? TFT_ORANGE : battery > 0.2 ? TFT_YELLOW : TFT_RED;
    batch.fillRectangle(x, (BAR_HEIGHT - ICON_BATTERY_HEIGHT) / 2, ICON_BATTERY_WIDTH, ICON_BATTERY_HEIGHT * (1 - battery), TFT_BLACK);
    batch.fillRectangle(x, (BAR_HEIGHT - ICON_BATTERY_HEIGHT) / 2 + ICON_BATTERY_HEIGHT * (1 - battery), ICON_BATTERY_WIDTH, ICON_BATTERY_HEIGHT * battery, batteryColor);
    batch.drawRectangle(x, (BAR_HEIGHT - ICON_BATTERY_HEIGHT) / 2, ICON_BATTERY_WIDTH, ICON_BATTERY_HEIGHT, TFT_WHITE);

    x -= STATUSBAR_SPACING;

    if (WiFi.status() == WL_CONNECTED) {
        x -= ICON_STATUSBAR_WIFI_WIDTH;
        batch.drawImage(&icon_wifi, x, (BAR_HEIGHT - ICON_STATUSBAR_WIFI_HEIGHT) / 2);
        x -= STATUSBAR_SPACING;
    }

    graphics.endBatch(&batch);

    vAppDelay(1000 / portTICK_PERIOD_MS);
}

void App_Statusbar::onClose() {
}

/*

*/