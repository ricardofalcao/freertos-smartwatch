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

	event_group = xEventGroupCreate();
	APPS_REGISTRY[APPS_REGISTRY_LENGTH++] = this;
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
			if (xEventGroupWaitBits(c->event_group, EVENT_RESUME, pdTRUE, pdTRUE, 0) & EVENT_RESUME) {
				c->resume();
			}
		} else {
			if (xEventGroupWaitBits(c->event_group, EVENT_MINIMIZE, pdTRUE, pdTRUE, 0) & EVENT_MINIMIZE) {
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

		xEventGroupSetBits(this->event_group, EVENT_RESUME);
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

	xEventGroupSetBits(event_group, EVENT_MINIMIZE_RES);

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
	TickType_t xLastWakeTime = xTaskGetTickCount();

	if (this->minimized) {
		if (xEventGroupWaitBits(event_group, EVENT_RESUME, pdTRUE, pdTRUE, xTicksToDelay) & EVENT_RESUME) {
			this->resume();
		}
	} else {
		if (xEventGroupWaitBits(event_group, EVENT_MINIMIZE, pdTRUE, pdTRUE, xTicksToDelay) & EVENT_MINIMIZE) {
			this->minimize();
		}
	}

	vTaskDelayUntil(&xLastWakeTime, xTicksToDelay);
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