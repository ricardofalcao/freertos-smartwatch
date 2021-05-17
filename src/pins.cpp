#include "pins.h"

#define BUZZER_PIN  22

Pins * _pins;

void isr_set_bit(SemaphoreHandle_t * handle) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(*handle, &xHigherPriorityTaskWoken);
    
    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

void isr_click_home() {
    isr_set_bit(&_pins->gpio0);
}

void pins_task(void * pvParameters) {
    Pins * t = (Pins *) pvParameters;
    _pins = t;

    ledcAttachPin(BUZZER_PIN, 0);

    pinMode(0, INPUT);
    attachInterrupt(digitalPinToInterrupt(0), isr_click_home, FALLING);

    while(true) {
        vTaskDelay(portMAX_DELAY);
    }
}

/*

*/

Pins::Pins() {
    buzzer_mutex = xSemaphoreCreateMutex();
    gpio0 = xSemaphoreCreateBinary();
}

void Pins::begin() {
    xTaskCreatePinnedToCore(
      pins_task,
      "Pins",
      1024,
      this,
      tskIDLE_PRIORITY + 1,
      NULL,
      0
  );
}