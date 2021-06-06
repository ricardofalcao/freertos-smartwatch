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

#define NOTIFICATION_WIDTH      80

App_Statusbar::App_Statusbar() : App(MSG_STATUSBAR_NAME, MSG_STATUSBAR_DESCRIPTION) {
    priority = 4;
    stack_depth = 4096;
    
    disableTouch = true;

    icon_wifi.setColorDepth(4);

    notification_sprite.setColorDepth(1);
    notification_sprite.setScrollRect(0, 0, NOTIFICATION_WIDTH, STATUSBAR_HEIGHT, TFT_BLACK);
    notification_sprite.setFreeFont(fonts[1]);
    notification_sprite.setTextSize(1);
    notification_sprite.setTextColor(TFT_WHITE, TFT_BLACK);
    notification_sprite.setTextDatum(ML_DATUM);
    notification_sprite.createSprite(NOTIFICATION_WIDTH * 2, STATUSBAR_HEIGHT);
}

void App_Statusbar::onOpen() {
    minimized = true;

    icon_wifi.createSprite(ICON_STATUSBAR_WIFI_WIDTH, ICON_STATUSBAR_WIFI_HEIGHT);
    icon_wifi.pushImage(0, 0, ICON_STATUSBAR_WIFI_WIDTH, ICON_STATUSBAR_WIFI_HEIGHT, (uint16_t *) icon_statusbar_wifi);
    icon_wifi.createPalette(icon_statusbar_wifi_palette, 16);

    GBatch_t batch = graphics.beginBatch(APP_VIEWPORT);
    batch.fillScreen(TFT_BLACK);
    graphics.endBatch(&batch);
}

void App_Statusbar::onTick() {
    GBatch_t batch = graphics.beginBatch(APP_VIEWPORT);

    /*
        Left Side
    */

    time_t now;
    time(&now);

    struct tm current_time;
    localtime_r(&now, &current_time);

    static bool hour_init = false;
    if (!hour_init || current_time.tm_hour != last_time.tm_hour || current_time.tm_min != last_time.tm_min) {
        hour_init = true;
        
        char timeBuffer[9];
        sprintf(timeBuffer, " %02d:%02d  ", current_time.tm_hour, current_time.tm_min);

        batch.drawFilledString(STATUSBAR_SPACING, BAR_HEIGHT / 2 - 2, timeBuffer, TFT_WHITE, TFT_BLACK, 2, ML_DATUM);
    }

    last_time = current_time;

    // Notifications

    if (current_notification_cursor > 0) {
        batch.drawImage(&notification_sprite, STATUSBAR_SPACING + 70, 0);

        notification_sprite.scroll(-3);
        current_notification_cursor = max(0, current_notification_cursor - 3);
    } else {
        Notification_t current_notification;
        if (notifications.popNotification(&current_notification)) {
            if (current_notification.message != NULL) {
                current_notification_cursor = notification_sprite.textWidth(current_notification.message);
            
                notification_sprite.drawString(current_notification.message, 0, STATUSBAR_HEIGHT / 2 - 2);
                vPortFree(current_notification.message);
            }
        }
    }

    /*
        Right Side
    */ 

    int32_t x = tft.width() - STATUSBAR_SPACING;

    /*x -= ICON_BATTERY_WIDTH;

    float battery = 0.6;
    
    if (abs(battery - last_battery) > 0.01) {
        last_battery = battery;
        
        uint32_t batteryColor = battery > 0.8 ? TFT_GREEN : battery > 0.5 ? TFT_ORANGE : battery > 0.2 ? TFT_YELLOW : TFT_RED;
        batch.fillRectangle(x, (BAR_HEIGHT - ICON_BATTERY_HEIGHT) / 2, ICON_BATTERY_WIDTH, ICON_BATTERY_HEIGHT * (1 - battery), TFT_BLACK);
        batch.fillRectangle(x, (BAR_HEIGHT - ICON_BATTERY_HEIGHT) / 2 + ICON_BATTERY_HEIGHT * (1 - battery), ICON_BATTERY_WIDTH, ICON_BATTERY_HEIGHT * battery, batteryColor);
        batch.drawRectangle(x, (BAR_HEIGHT - ICON_BATTERY_HEIGHT) / 2, ICON_BATTERY_WIDTH, ICON_BATTERY_HEIGHT, TFT_WHITE);
    }*/

    x -= STATUSBAR_SPACING;

    x -= ICON_STATUSBAR_WIFI_WIDTH;
    
    static bool wifi_connected = true;
    if (wifi_connected && WiFi.status() != WL_CONNECTED) {
        wifi_connected = false;

        batch.drawImage(&icon_wifi, x, (BAR_HEIGHT - ICON_STATUSBAR_WIFI_HEIGHT) / 2);
        batch.drawLine(x + 4, (BAR_HEIGHT - ICON_STATUSBAR_WIFI_HEIGHT) / 2 + 4, x + ICON_STATUSBAR_WIFI_WIDTH - 4, (BAR_HEIGHT + ICON_STATUSBAR_WIFI_HEIGHT) / 2 - 4, TFT_RED, 2);
    } else if (!wifi_connected && WiFi.status() == WL_CONNECTED) {
        wifi_connected = true;
        
        batch.drawImage(&icon_wifi, x, (BAR_HEIGHT - ICON_STATUSBAR_WIFI_HEIGHT) / 2);
    }

    x -= STATUSBAR_SPACING;

    graphics.endBatch(&batch);

    vAppDelay(100 / portTICK_PERIOD_MS);
    
}

void App_Statusbar::onClose() {
}

/*

*/