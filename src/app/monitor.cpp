#include "app/monitor.h"
#include "tft.h"
#include "graphics.h"

#define MAXIMUM_TASKS   20

App_Monitor::App_Monitor() : App("Monitor", "System Monitor") {
    priority = 3;
    stack_depth = 10240;
}

void App_Monitor::fillTasks() {
   
}

void App_Monitor::onOpen() {
    graphics.fillScreen(TFT_WHITE);

    fillTasks();
}

void App_Monitor::onTick() {
}

void App_Monitor::onTouchTick() {
    TouchData data = touch.waitData();
}

void App_Monitor::onClose() {
    
}
