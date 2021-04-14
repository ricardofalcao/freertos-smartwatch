#include "app/clock.h"

#include "touch.h"
#include "graphics.h"

App_Clock::App_Clock() : App("Clock", "Shows the current time") {
    priority = 3;
    stack_depth = 1024;
}

void App_Clock::onOpen() {
    Serial.println("[Clock] OPEN");

    graphics.drawCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 100, TFT_BLACK);
    graphics.drawCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 99, TFT_BLACK);
    graphics.drawCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 98, TFT_BLACK);
    graphics.drawCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 97, TFT_BLACK);

    graphics.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 3, TFT_BLACK);
}

void _draw_line(uint8_t seconds, uint32_t color) {
    float angle = (seconds % 60) * 360 / 60;
    float size = 95;

    int32_t x = size * cos(-angle);
    int32_t y = size * sin(-angle);

    graphics.drawLine(TFT_WIDTH / 2, TFT_HEIGHT / 2, TFT_WIDTH / 2 + x, TFT_HEIGHT / 2 + y, color);
}

void App_Clock::onTick() {
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

    _draw_line(seconds - 1, TFT_WHITE);
    graphics.fillCircle(TFT_WIDTH / 2, TFT_HEIGHT / 2, 3, TFT_BLACK);
    _draw_line(seconds, TFT_BLACK);

    seconds++;
    vTaskDelay(100 / portTICK_RATE_MS);
}

void App_Clock::onClose() {
    Serial.println("[Clock] Closed");
}