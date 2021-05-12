#include "app/clock.h"

#include "wifi.h"
#include "time.h"

#include "touch.h"
#include "graphics.h"

App_Clock::App_Clock() : App("Clock", "Shows the current time") {
    priority = 3;
    stack_depth = 10240;
}

void App_Clock::onOpen() {
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    batch.fillScreen(TFT_WHITE);
    batch.drawCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 99, TFT_BLACK, 5);
    batch.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 3, TFT_BLACK);

    graphics.endBatch(&batch);
}

void App_Clock::onResume() {
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    batch.fillScreen(TFT_WHITE);
    batch.drawCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 99, TFT_BLACK, 5);
    batch.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 3, TFT_BLACK);

    graphics.endBatch(&batch);
}

void App_Clock::draw_line(GBatch_t * batch, float angle, float size, uint32_t color) {

    int32_t x = size * cos(angle);
    int32_t y = size * sin(angle);

    batch->drawLine(TFT_WIDTH / 2, TFT_HEIGHT / 2, TFT_WIDTH / 2 + x, TFT_HEIGHT / 2 + y, color);
}

void App_Clock::onTick() {
    
    if (!minimized) {
        GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

        old_time = current_time;

        time_t now;
        time(&now);
        localtime_r(&now, &current_time);
          Serial.println(&current_time, "%A, %B %d %Y %H:%M:%S");

        float angle_s = old_time.tm_sec * 0.1047 - 1.5708;
        draw_line(&batch, angle_s, 96, TFT_WHITE);

        float angle_m = old_time.tm_min * 0.1047 - 1.5708;
        draw_line(&batch, angle_m, 70, TFT_WHITE);

        float angle_h = ((old_time.tm_hour % 12) + old_time.tm_min / 60.0) * 0.5236 - 1.5708;
        draw_line(&batch, angle_h, 50, TFT_WHITE);

        angle_s = current_time.tm_sec * 0.1047 - 1.5708;
        draw_line(&batch, angle_s, 96, TFT_BLACK);

        angle_m = current_time.tm_min * 0.1047 - 1.5708;
        draw_line(&batch, angle_m, 70, TFT_BLACK);

        angle_h = ((current_time.tm_hour % 12) + current_time.tm_min / 60.0) * 0.5236 - 1.5708;
        draw_line(&batch, angle_h, 50, TFT_BLACK);

        batch.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 3, TFT_BLACK);

        graphics.endBatch(&batch);
    }

    vAppDelay(1000 / portTICK_RATE_MS);
}

void App_Clock::onClose() {
}