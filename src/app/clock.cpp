#include "app/clock.h"

#include "esp_wifi.h"
#include "time.h"

#include "touch.h"
#include "graphics.h"

App_Clock::App_Clock() : App("Clock", "Shows the current time") {
    priority = 3;
    stack_depth = 10240;
}

void App_Clock::onOpen() {
    graphics.fillScreen(TFT_WHITE);
    graphics.drawCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 99, TFT_BLACK, 5);
    graphics.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 3, TFT_BLACK);
}

void _draw_line(float angle, float size, uint32_t color) {

    int32_t x = size * cos(angle);
    int32_t y = size * sin(angle);

    graphics.drawLine(TFT_WIDTH / 2, TFT_HEIGHT / 2, TFT_WIDTH / 2 + x, TFT_HEIGHT / 2 + y, color);
}

void App_Clock::onTick() {
    old_time = current_time;
    if(!getLocalTime(&current_time)){
        esp_Wsync_time();
    }

    TouchData touchData = touch.getData();

    if (touchData.pressed) {
        int32_t centerX = TFT_WIDTH / 2;
        int32_t centerY = TFT_HEIGHT / 2;

        int32_t dx = (touchData.x - centerX);
        int32_t dy = (touchData.y - centerY);

        float distanceSq = dx*dx + dy*dy;
        
        if (distanceSq <= 96*96) {
            graphics.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 96, TFT_RED);
        }

    } else  {
        graphics.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 96, TFT_WHITE);
    }

    float angle_s = old_time.tm_sec * 0.1047 - 1.5708;
    _draw_line(angle_s, 96, TFT_WHITE);

    float angle_m = old_time.tm_min * 0.1047 - 1.5708;
    _draw_line(angle_m, 70, TFT_WHITE);

    float angle_h = ((old_time.tm_hour % 12) + old_time.tm_min / 60.0) * 0.5236 - 1.5708;
    _draw_line(angle_h, 50, TFT_WHITE);

    angle_s = current_time.tm_sec * 0.1047 - 1.5708;
    _draw_line(angle_s, 96, TFT_BLACK);

    angle_m = current_time.tm_min * 0.1047 - 1.5708;
    _draw_line(angle_m, 70, TFT_BLACK);

    angle_h = ((current_time.tm_hour % 12) + current_time.tm_min / 60.0) * 0.5236 - 1.5708;
    _draw_line(angle_h, 50, TFT_BLACK);

    graphics.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 3, TFT_BLACK);

    vTaskDelay(1000 / portTICK_RATE_MS);
}

void App_Clock::onClose() {
}