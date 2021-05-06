#include "app/app.h"

#include "touch.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif


App::App(String _name, String _description) {
    name = _name;
    description = _description;
    color = rand();
}

void _run_app_task(void *pvParameters)
{
  App * c = (App *) pvParameters;
  
  c->running = true;
  graphics.setViewport(c->viewport);

  c->onOpen();
  c->startTouchTask();

  while(true) {
    graphics.setViewport(c->viewport);
    c->onTick();
  }
}

void _run_app_touch_task(void *pvParameters)
{
  App * c = (App *) pvParameters;

  while(true) {
    graphics.setViewport(c->viewport);
    c->onTouchTick();
  }
}

/*

*/

void App::open(bool _minimized) {
    Serial.printf("[App] Start '%s' main task\n", name.c_str());
    minimized = _minimized;
    
    xTaskCreatePinnedToCore(
        _run_app_task,
        name.c_str(),
        stack_depth,
        this,
        tskIDLE_PRIORITY + 5 + priority,
        &task_handle,
        target_core
    );
}

bool App::close() {
    if(!running) {
        return false;
    }

    Serial.printf("[App] Closing '%s'\n", name.c_str());
    this->running = false;
    this->onClose();
    vTaskDelete(touch_task_handle);
    vTaskDelete(task_handle);
    return true;
}

void App::startTouchTask() {
  Serial.printf("[App] Start '%s' touch task\n", name.c_str());
  xTaskCreatePinnedToCore(
      _run_app_touch_task,
      (name + "-TOUCH").c_str(),
      touch_stack_depth,
      this,
      tskIDLE_PRIORITY + 5 + touch_priority,
      &touch_task_handle,
      target_core
  );
}