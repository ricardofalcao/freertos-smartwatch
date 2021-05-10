#include "app/app.h"

#include "touch.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

App::App(String _name, String _description)
{
	name = _name;
	description = _description;
	color = rand();

	resume_signal = xSemaphoreCreateBinary();
}

void _run_app_task(void *pvParameters)
{
	App *c = (App *)pvParameters;

	c->running = true;
	c->minimized = false;

	graphics.setViewport(c->viewport);

	c->onOpen();
	c->startTouchTask();

	while (true)
	{
		if (c->minimized) {
			if (xSemaphoreTake(c->resume_signal, 0) == pdTRUE) {
				c->open();
			}
		} else {
			if (xSemaphoreTake(c->minimize_signal, 0) == pdTRUE) {
				c->minimize();
			}
		}

		graphics.setViewport(c->viewport);
		c->onTick();
	}
}

void _run_app_touch_task(void *pvParameters)
{
	App *c = (App *)pvParameters;

	while (true)
	{
		graphics.setViewport(c->viewport);
		c->onTouchTick();
	}
}

/*

*/

void App::open()
{
	if (this->running)
	{
		if(!this->minimized) {
			return;
		}

		this->resume();
		return;
	}

	Serial.printf("[App] Start '%s' main task\n", name.c_str());

	xTaskCreatePinnedToCore(
		_run_app_task,
		name.c_str(),
		stack_depth,
		this,
		tskIDLE_PRIORITY + 5 + priority,
		&task_handle,
		target_core);
}

void App::minimize()
{
	if (!canMinimize) {
		close();
		return;
	}

	Serial.printf("[App] Minimizing '%s'\n", name.c_str());
	this->minimized = true;
	this->onMinimize();
	vTaskDelete(touch_task_handle);
}

void App::resume()
{
	Serial.printf("[App] Resuming '%s' main task\n", name.c_str());

	this->minimized = false;
	this->onResume();
	this->startTouchTask();
}

bool App::close()
{
	if (!running)
	{
		return false;
	}

	Serial.printf("[App] Closing '%s'\n", name.c_str());
	this->running = false;
	this->onClose();
	vTaskDelete(touch_task_handle);
	vTaskDelete(task_handle);
	return true;
}

void App::vAppDelay(const TickType_t xTicksToDelay) {
	if (this->minimized) {
		if (xSemaphoreTake(resume_signal, xTicksToDelay) == pdTRUE) {
			this->open();
		}
	} else {
		if (xSemaphoreTake(minimize_signal, xTicksToDelay) == pdTRUE) {
			this->minimize();
		}
	}
}

void App::startTouchTask()
{
	Serial.printf("[App] Start '%s' touch task\n", name.c_str());
	xTaskCreatePinnedToCore(
		_run_app_touch_task,
		(name + "-TOUCH").c_str(),
		touch_stack_depth,
		this,
		tskIDLE_PRIORITY + 5 + touch_priority,
		&touch_task_handle,
		target_core);
}