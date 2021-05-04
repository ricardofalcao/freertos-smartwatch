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
    uint32_t ulTotalRunTime;

    /* Generate raw status information about each task. */
    UBaseType_t uxArraySize = uxTaskGetSystemState(statuses, MONITOR_TASKS_LENGTH, &ulTotalRunTime );

    /* For percentage calculations. */
    ulTotalRunTime /= 100;

    /* Avoid divide by zero errors. */
    if( ulTotalRunTime > 0 )
    {
        qsort(statuses, uxArraySize, sizeof(TaskStatus_t), sort_desc);

        /* For each populated position in the pxTaskStatusArray array,
        format the raw data as human readable ASCII data. */
        for( UBaseType_t x = 0; x < MONITOR_TASKS_LENGTH; x++ )
        {
            int32_t y = x * 10 + 10;
            graphics.fillRectangle(10, y, 200, 10, TFT_WHITE);
            if (x >= uxArraySize) {
                break;
            }

            uint32_t color = TFT_BLACK;

            /* What percentage of the total run time has the task used?
            This will always be rounded down to the nearest integer.
            ulTotalRunTimeDiv100 has already been divided by 100. */
            uint32_t ulStatsAsPercentage = statuses[ x ].ulRunTimeCounter / ulTotalRunTime;

            graphics.drawString(10, y, statuses[ x ].pcTaskName, color, 1, TL_DATUM);

            char buffer[8];

            if (ulStatsAsPercentage > 0UL) {
                sprintf(buffer, "%d %%", ulStatsAsPercentage);
            } else {
                sprintf(buffer, "<1 %%");
            }
            
            graphics.drawString(130, y, buffer, color, 1, TR_DATUM);

            sprintf(buffer, "%d", statuses[x].usStackHighWaterMark);
            graphics.drawString(170, y, buffer, color, 1, TR_DATUM);
        }
    }
}

void App_Monitor::onOpen() {
    graphics.fillScreen(TFT_WHITE);
}

void App_Monitor::onTick() {
    fillTasks();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void App_Monitor::onTouchTick() {
    vTaskDelay(portMAX_DELAY);
}

void App_Monitor::onClose() {
    
}
