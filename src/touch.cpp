#include "touch.h"
#include "tft.h"

#include <FS.h>

//#define TOUCH_DEBUG

#define ALWAYS_CALIBRATE    false
#define CALIBRATION_FILE    "/TouchCal"

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
      10,
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

        TouchData oldData;
        xQueueReceive(data_queue, &oldData, 0);

        if (oldData.pressed != data.pressed || oldData.x != data.x || oldData.y != data.y) {
            xQueueSendToFront(data_queue, &data, 0);
        }

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

TouchData Touch::getData() {
    TouchData data;
    xQueuePeek(data_queue, &data, 0);
    return data;
}

TouchData Touch::waitData() {
    TouchData data;
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

bool RectangleTouchListener::contains(TouchData data) {
    return (x <= data.x && y <= data.y) && (data.x < (x + width) && data.y < (y + height));
}

CircleTouchListener::CircleTouchListener(int32_t _x, int32_t _y, int32_t _radius) {
    x = _x;
    y = _y;
    radius = _radius;
}

bool CircleTouchListener::contains(TouchData data) {
    int32_t dx = data.x - x;
    int32_t dy = data.y - y;

    return (dx * dx + dy*dy) <= (radius * radius);
}