#include <Arduino.h>

#include <FS.h>

#include "tft.h"
#include "touch.h"
#include "graphics.h"

#include "app/clock.h"
#include "app/alert.h"

Touch touch;
Graphics graphics;

App_Clock clock_app;
App_Alert alert_app;

void setup() {
  Serial.begin(115200);

  Serial.println("[Main] Initializing");
  
  // check file system exists
  if (!SPIFFS.begin()) {
      Serial.println("[MAIN] Formating file system...");
      SPIFFS.format();
      SPIFFS.begin();
  }

  spi_mutex = xSemaphoreCreateMutex();
  tft.init();

  touch.calibrate();
  tft.fillScreen(TFT_WHITE);

  touch.begin();
  graphics.begin();

  clock_app.open();
  //alert_app.open();
}

void loop() {
}