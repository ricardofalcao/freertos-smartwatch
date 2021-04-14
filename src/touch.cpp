#include "touch.h"
#include "tft.h"

#include <FS.h>

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
      1024,
      this,
      2,
      NULL,
      0
  );
}

void Touch::onTick() {
    if (xSemaphoreTake(spi_mutex, portMAX_DELAY) == pdTRUE) {
        TouchData data;
        data.pressed = tft.getTouch(&data.x, &data.y);
        xSemaphoreGive(spi_mutex);

        xQueueOverwrite(data_queue, &data);

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

TouchData Touch::getData() {
    TouchData data;
    xQueuePeek(data_queue, &data, 0);
    return data;
}