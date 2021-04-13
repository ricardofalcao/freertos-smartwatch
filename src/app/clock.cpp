#include "app/clock.h"

#include "graphics.h"

App_Clock::App_Clock() : App("Clock", "Shows the current time") {
    priority = 1;
    stack_depth = 1024;
}

void App_Clock::onOpen() {
    Serial.println("[Clock] OPEN");
}

void App_Clock::onTick() {
    Serial.println("[Clock] Tick");
    graphics.drawRectangle(0, 0, 10, 10, TFT_RED);
    vTaskDelay(1000 / portTICK_RATE_MS);
}

void App_Clock::onClose() {
    Serial.println("[Clock] Closed");
}