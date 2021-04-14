#include "app/alert.h"


App_Alert::App_Alert() : App("Alert", "Schedules and event") {
    priority = 3;
    stack_depth = 1024;
}

unsigned long eventTime = 0;
void App_Alert::onOpen() {
    Serial.println("[Alert] OPEN");

    eventTime = millis() + 10500;
}

void App_Alert::onTick() {
    unsigned long timeLeft = eventTime - millis();
    if (timeLeft <= 0) {
        Serial.println("[Alert] ALERT!");
    }

    vTaskDelay(timeLeft / portTICK_PERIOD_MS);
}

void App_Alert::onClose() {
    Serial.println("[Alert] Closed");
}