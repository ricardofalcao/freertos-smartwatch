#include <Arduino.h>
#include "app/app.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif


App::App(String _name, String _description) {
    name = _name;
    description = _description;
}

void _run_app_task(void *pvParameters)
{
  App * c = (App *) pvParameters;
  
  c->running = true;
  c->onOpen();

  while(true) {
    c->onTick();
  }
}

void _run_app_touch_task(void *pvParameters)
{
  App * c = (App *) pvParameters;

  while(true) {
    c->onTouchTick();
  }
}

/*

*/

void App::open() {
    xTaskCreatePinnedToCore(
        _run_app_task,
        name.c_str(),
        stack_depth,
        this,
        priority,
        &task_handle,
        target_core
    );

    char touch_name[32];
    sprintf(touch_name, "%s-TOUCH", name.c_str());

    xTaskCreatePinnedToCore(
        _run_app_touch_task,
        touch_name,
        touch_stack_depth,
        this,
        touch_priority,
        &touch_task_handle,
        target_core
    );
}

bool App::close() {
    if(!running) {
        return false;
    }

    this->running = false;
    this->onClose();
    vTaskDelete(touch_task_handle);
    vTaskDelete(task_handle);
    return true;
}