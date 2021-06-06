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
	
	if (!c->disableTouch) {
		c->startTouchTask();
	}

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

	const char * name_c = LANG_EN.messages[name];
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
	
	if (!disableTouch) {
		vTaskDelete(touch_task_handle);
	}
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
	
	if (!disableTouch) {
		vTaskDelete(touch_task_handle);
	}

	vTaskDelete(task_handle);
	return true;
}

EventBits_t App::vAppConditionalDelay(const TickType_t xTicksToDelay, EventBits_t additional_bits) {
	TickType_t xLastWakeTime = xTaskGetTickCount();

	if (this->minimized) {
		EventBits_t bits = xEventGroupWaitBits(event_group, EVENT_RESUME | additional_bits, pdFALSE, pdFALSE, xTicksToDelay);

		if (bits & EVENT_RESUME) {
			xEventGroupClearBits(event_group, EVENT_RESUME);
			this->resume();
		} 

		if (bits) {
			return bits;
		}

	} else {
		EventBits_t bits = xEventGroupWaitBits(event_group, EVENT_MINIMIZE | additional_bits, pdFALSE, pdFALSE, xTicksToDelay);
		
		if (bits & EVENT_MINIMIZE) {
			xEventGroupClearBits(event_group, EVENT_MINIMIZE);
			this->minimize();
		}

		if (bits) {
			return bits;
		}
	}

	TickType_t xNowWakeTime = xTaskGetTickCount();

	if (xNowWakeTime - xLastWakeTime < xTicksToDelay) {
		return vAppConditionalDelay(xTicksToDelay - (xNowWakeTime - xLastWakeTime), additional_bits);
	}

	return 0;
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

	if (xNowWakeTime - xLastWakeTime < xTicksToDelay) {
		vAppDelay(xTicksToDelay - (xNowWakeTime - xLastWakeTime));
	}
}

void App::startTouchTask()
{
	const char * name_c = LANG_EN.messages[name];

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