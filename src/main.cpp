#include <Arduino.h>

#include <FS.h>
#include <WiFi.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include "tft.h"
#include "touch.h"
#include "graphics.h"
#include "esp_wifi.h"

#include "app/clock.h"
#include "app/alert.h"
#include "app/tictactoe.h"

#define WIFI_NETWORK "Vodafone-284C30"
#define WIFI_PASS "tYUREqcuVn"

Touch touch;
Graphics graphics;

App_Clock clock_app;
App_Alert alert_app;

App_TicTacToe tictactoe_app;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

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

  //esp_Wconnect(WIFI_NETWORK, WIFI_PASS, 10000);
  //esp_Wsync_time();

  touch.begin();
  graphics.begin();

  //clock_app.open();
  //alert_app.open();
  tictactoe_app.open();

}

void loop() {
}