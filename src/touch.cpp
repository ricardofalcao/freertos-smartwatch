#include "touch.h"
#include "tft.h"

#include <FS.h>
#include <SPIFFS.h>

//#define TOUCH_DEBUG

#define ALWAYS_CALIBRATE    false
#define CALIBRATION_FILE    "/TouchCali"

#define PRESSED_BIT         0x01
#define RELEASED_BIT        0x02

void touch_task(void * pvParameters) {
    Touch * t = (Touch *) pvParameters;
    while(true) {
        t->onTick();
    }
}

/*

*/

Touch::Touch() {
    data_queue = xQueueCreate(1, sizeof(TouchData));
    event_group = xEventGroupCreate();
}

void Touch::calibrate() {
    uint16_t calData[5];
    uint8_t calDataOK = 0;

    // check if calibration file exists and size is correct
    if (SPIFFS.exists(CALIBRATION_FILE)) {
        fs::File f = SPIFFS.open(CALIBRATION_FILE, "r");

        if (f) {
            calDataOK = f.readBytes((char *) calData, 14) == 14;
            f.close();
        }
    }

    if (calDataOK && !ALWAYS_CALIBRATE) {
        Serial.println("[Touch] Found calibration file...");
        tft.setTouch(calData);
        return;
    }

    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();
    ledcWrite(4, 255);

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    fs::File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
        f.write((const unsigned char *) calData, 14);
        f.close();
    }
}

void Touch::begin() {
    xTaskCreatePinnedToCore(
      touch_task,
      "Touch",
      4096,
      this,
      tskIDLE_PRIORITY + 2,
      NULL,
      0
  );
}

void Touch::onTick() {
    if (xSemaphoreTake(spi_mutex, portMAX_DELAY) == pdTRUE) {
        TouchData data;
        data.pressed = tft.getTouch(&data.x, &data.y);

#ifdef TOUCH_DEBUG
        Serial.printf("%d %d %d\n", data.x, data.y, data.pressed);
#endif

        xSemaphoreGive(spi_mutex);
        
        if (data.pressed && !oldData.pressed) {
            xQueueOverwrite(data_queue, &data);

            xEventGroupClearBits(event_group, RELEASED_BIT);
            xEventGroupSetBits(event_group, PRESSED_BIT);
        } else if (!data.pressed && oldData.pressed) {
            xEventGroupClearBits(event_group, PRESSED_BIT);
            xEventGroupSetBits(event_group, RELEASED_BIT);
        
            data = oldData;
            data.pressed = false;

            xQueueOverwrite(data_queue, &data);
        } else if (data.pressed) {

            xQueueOverwrite(data_queue, &data);
        }

        oldData = data;

        vTaskDelay(1000 / TOUCH_SAMPLE_RATE_HZ / portTICK_RATE_MS);
    }
}

TouchData Touch::get() {
    TouchData data;
    xQueuePeek(data_queue, &data, 0);
    return data;
}

TouchData Touch::waitRelease() {
    TouchData data;

    xEventGroupWaitBits(event_group, PRESSED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    xEventGroupWaitBits(event_group, RELEASED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    xQueuePeek(data_queue, &data, portMAX_DELAY);

    return data;
}

TouchData Touch::waitPress() {
    TouchData data;

    xEventGroupWaitBits(event_group, PRESSED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    xQueuePeek(data_queue, &data, portMAX_DELAY);
    return data;
}

/*

*/

RectangleTouchListener::RectangleTouchListener(int32_t _x, int32_t _y, int32_t _width, int32_t _height) {
    x = _x;
    y = _y;
    width = _width;
    height = _height;
}

bool RectangleTouchListener::contains(GViewport_t viewport, TouchData data) {
    int32_t _x = x + viewport.x;
    int32_t _y = y + viewport.y;

    return (_x <= data.x && _y <= data.y) && (data.x < (_x + width) && data.y < (_y + height));
}

CircleTouchListener::CircleTouchListener(int32_t _x, int32_t _y, int32_t _radius) {
    x = _x;
    y = _y;
    radius = _radius;
}

bool CircleTouchListener::contains(GViewport_t viewport, TouchData data) {
    int32_t _x = x + viewport.x;
    int32_t _y = y + viewport.y;

    int32_t dx = data.x - _x;
    int32_t dy = data.y - _y;

    return (dx * dx + dy*dy) <= (radius * radius);
}