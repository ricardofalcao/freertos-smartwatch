#include <Arduino.h>
#include "app/app.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif


App::App(String _name, String _description, int _priority) {
    name = _name;
    description = _description;
    priority = _priority;
}

void run_app_task(void *pvParameters)
{
  App * c = (App *) pvParameters;
  
  c->running = true;
  c->onOpen();

  while(true) {
    c->onTick();
  }
}

void App::open() {
    xTaskCreatePinnedToCore(
        run_app_task,
        name.c_str(),
        1024,
        this,
        1,
        &task_handle,
        ARDUINO_RUNNING_CORE
    );
}

bool App::close() {
    if(!running) {
        return false;
    }

    this->running = false;
    this->onClose();
    vTaskDelete(task_handle);
    return true;
}