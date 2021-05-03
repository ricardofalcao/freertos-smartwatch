#include "app/monitor.h"
#include "tft.h"
#include "graphics.h"

#define MAXIMUM_TASKS   20

App_Monitor::App_Monitor() : App("Monitor", "System Monitor") {
    priority = 3;
    stack_depth = 10240;
}

void App_Monitor::fillTasks() {
   /* Take a snapshot of the number of tasks in case it changes while this
   function is executing. */
   UBaseType_t uxArraySize = uxTaskGetNumberOfTasks();

   /* Allocate a TaskStatus_t structure for each task.  An array could be
   allocated statically at compile time. */
   TaskStatus_t * pxTaskStatusArray = (TaskStatus_t *) pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

   if( pxTaskStatusArray != NULL )
   {
        uint32_t ulTotalRunTime;

        /* Generate raw status information about each task. */
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime );

        /* For percentage calculations. */
        ulTotalRunTime /= 100;

        /* Avoid divide by zero errors. */
        if( ulTotalRunTime > 0 )
        {
            char pcWriteBuffer[256];

            /* For each populated position in the pxTaskStatusArray array,
            format the raw data as human readable ASCII data. */
            for( UBaseType_t x = 0; x < uxArraySize; x++ )
            {
                /* What percentage of the total run time has the task used?
                This will always be rounded down to the nearest integer.
                ulTotalRunTimeDiv100 has already been divided by 100. */
                uint32_t ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;

                if( ulStatsAsPercentage > 0UL )
                {
                    sprintf( pcWriteBuffer, "%stt%utt%u%%rn",
                                        pxTaskStatusArray[ x ].pcTaskName,
                                        pxTaskStatusArray[ x ].ulRunTimeCounter,
                                        ulStatsAsPercentage );
                }
                else
                {
                    /* If the percentage is zero here then the task has
                    consumed less than 1% of the total run time. */
                    sprintf( pcWriteBuffer, "%stt%utt<1%%rn",
                                        pxTaskStatusArray[ x ].pcTaskName,
                                        pxTaskStatusArray[ x ].ulRunTimeCounter );
                }

                graphics.drawString(0, x * 20, pcWriteBuffer, TFT_BLACK, 3, TL_DATUM);
            }
        }

        /* The array is no longer needed, free the memory it consumes. */
        vPortFree( pxTaskStatusArray );
   }
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
