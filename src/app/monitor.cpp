#include "app/monitor.h"
#include "tft.h"
#include "graphics.h"

#define MAXIMUM_TASKS   20

App_Monitor::App_Monitor() : App("Monitor", "System Monitor") {
    priority = 3;
    stack_depth = 10240;
}

// qsort requires you to create a sort function
int sort_desc(const void *cmp1, const void *cmp2)
{
  TaskStatus_t * a = (TaskStatus_t *) cmp1;
  TaskStatus_t * b = (TaskStatus_t *) cmp2;
  // The comparison
  return a->uxCurrentPriority > b->uxCurrentPriority ? -1 : (a->uxCurrentPriority < b->uxCurrentPriority ? 1 : strcmp(a->pcTaskName, b->pcTaskName));
}

void App_Monitor::fillTasks() {
    if(this->minimized) {
        return;
    }

    uint32_t ulTotalRunTime;

    /* Generate raw status information about each task. */
    UBaseType_t uxArraySize = uxTaskGetSystemState(statuses, MONITOR_TASKS_LENGTH, &ulTotalRunTime );

    /* For percentage calculations. */
    ulTotalRunTime /= 100;

    /* Avoid divide by zero errors. */
    if( ulTotalRunTime > 0 )
    {
        qsort(statuses, uxArraySize, sizeof(TaskStatus_t), sort_desc);

        graphics.drawString(10, 10, "Task", TFT_BLACK, 1, TL_DATUM);
        graphics.drawString(160, 10, "CPU", TFT_BLACK, 1, TR_DATUM);
        graphics.drawString(200, 10, "MEM", TFT_BLACK, 1, TR_DATUM);

        /* For each populated position in the pxTaskStatusArray array,
        format the raw data as human readable ASCII data. */
        for( UBaseType_t x = 0; x < MONITOR_TASKS_LENGTH; x++ )
        {
            int32_t y = (x + 1) * 16 + 10;
            if (x >= uxArraySize) {
                graphics.fillRectangle(10, y, 200, 16, TFT_WHITE);
                continue;
            }

            /* What percentage of the total run time has the task used?
            This will always be rounded down to the nearest integer.
            ulTotalRunTimeDiv100 has already been divided by 100. */
            uint32_t ulStatsAsPercentage = statuses[ x ].ulRunTimeCounter / ulTotalRunTime;

            uint32_t color = ulStatsAsPercentage > 50 ? TFT_RED : ulStatsAsPercentage > 25 ? TFT_ORANGE : TFT_DARKGREY;

            char nameBuffer[24] = {' '};
            strcpy(nameBuffer, statuses[x].pcTaskName);

            graphics.drawFilledString(10, y, nameBuffer, color, TFT_WHITE, 1, TL_DATUM);

            char buffer[8] = {' '};

            if (ulStatsAsPercentage > 0UL) {
                sprintf(buffer, "%d %%", ulStatsAsPercentage);
            } else {
                sprintf(buffer, "<1 %%");
            }
            
            graphics.drawFilledString(160, y, buffer, color, TFT_WHITE, 1, TR_DATUM);

            char buffer2[8] = {' '};
            sprintf(buffer2, "%d", statuses[x].usStackHighWaterMark);
            graphics.drawFilledString(200, y, buffer2, color, TFT_WHITE, 1, TR_DATUM);
        }
    }
}

void App_Monitor::onOpen() {
    graphics.fillScreen(TFT_WHITE);
}

void App_Monitor::onResume() {
    graphics.fillScreen(TFT_WHITE);
}

void App_Monitor::onTick() {
    fillTasks();
    vAppDelay(1000 / portTICK_PERIOD_MS);
}

void App_Monitor::onTouchTick() {
    vTaskDelay(portMAX_DELAY);
}

void App_Monitor::onClose() {
    
}
