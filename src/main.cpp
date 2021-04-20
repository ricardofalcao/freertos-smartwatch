#include <Arduino.h>

#include <FS.h>
#include <WiFi.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include "tft.h"
#include "touch.h"
#include "graphics.h"
#include "esp_wifi.h"

#include "app/drawer.h"
#include "app/clock.h"
#include "app/alert.h"
#include "app/tictactoe.h"
#include "app/metronome.h"

#define WIFI_NETWORK "Vodafone-284C30"
#define WIFI_PASS "tYUREqcuVn"

Touch touch;
Graphics graphics;

App_Drawer drawer_app;

App_Clock clock_app;
App_Alert alert_app;
App_Metronome metronome_app;

App_TicTacToe tictactoe_app;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

  Serial.begin(115200);

  Serial.println("[Main] Initializing SPIFFS");
  // check file system exists
  SPIFFS.begin(true);

  spi_mutex = xSemaphoreCreateMutex();
  Serial.println("[Main] Initializing TFT");
  tft.init();

  Serial.println("[Main] Calibration Touch");
  touch.calibrate();

  //esp_Wconnect(WIFI_NETWORK, WIFI_PASS, 10000);
  //esp_Wsync_time();

  Serial.println("[Main] Initializing Touch");
  touch.begin();
  Serial.println("[Main] Initializing Graphics");
  graphics.begin();

  drawer_app.addApp(&clock_app);
  drawer_app.addApp(&alert_app);
  drawer_app.addApp(&tictactoe_app);
  drawer_app.addApp(&metronome_app);
  
  drawer_app.open(false);
}

void loop() {
}