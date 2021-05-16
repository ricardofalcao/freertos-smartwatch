#include "app/app.h"

#include "touch.h"

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

App::App(Message_t _name, Message_t _description)
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

		c->onTick();
	}
}

void _run_app_touch_task(void *pvParameters)
{
	App *c = (App *)pvParameters;

	while (true)
	{
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

	const char * name_c = lang.get(name);
	Serial.printf("[App] Start '%s' main task\n", name_c);

	xTaskCreatePinnedToCore(
		_run_app_task,
		name_c,
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

	const char * name_c = lang.get(name);
	Serial.printf("[App] Minimizing '%s'\n", name_c);
	this->minimized = true;
	this->onMinimize();
	vTaskDelete(touch_task_handle);
}

void App::resume()
{
	const char * name_c = lang.get(name);
	Serial.printf("[App] Resuming '%s' main task\n", name_c);

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

	const char * name_c = lang.get(name);
	Serial.printf("[App] Closing '%s'\n", name_c);
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

	TickType_t xNowWakeTime = xTaskGetTickCount();
	TickType_t delayTicks = xTicksToDelay - (xNowWakeTime - xLastWakeTime);
	
	if (delayTicks > 0) {
		vAppDelay(delayTicks);
	}
}

void App::startTouchTask()
{
	const char * name_c = lang.get(name);

	char touchname_c[24];
	sprintf(touchname_c, "%s-TOUCH", name_c);

	Serial.printf("[App] Start '%s' touch task\n", name_c);
	xTaskCreatePinnedToCore(
		_run_app_touch_task,
		touchname_c,
		touch_stack_depth,
		this,
		tskIDLE_PRIORITY + 5 + touch_priority,
		&touch_task_handle,
		target_core);
}