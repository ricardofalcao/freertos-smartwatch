#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "touch.h"
#include "app/app.h"

#define MONITOR_TASKS_LENGTH 30

class App_Monitor : public App {
    private:
        TaskStatus_t statuses[MONITOR_TASKS_LENGTH];

    public:
        App_Monitor();

        void onOpen() override;   

        void onResume() override;   

        void onTick() override;   

        void onClose() override;   

    private:
        void fillTasks();
};