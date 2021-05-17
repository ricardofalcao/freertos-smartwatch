#include "sdkconfig.h"
#include <Arduino.h>

#include <FS.h>
#include <SPIFFS.h>

#include <WiFi.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include "tft.h"
#include "touch.h"
#include "graphics.h"
#include "wifi.h"
#include "pins.h"
#include "notifications.h"

#include "app/drawer.h"
#include "app/status_bar.h"
#include "app/monitor.h"
#include "app/lang.h"
#include "app/clock.h"
#include "app/weather.h"
#include "app/tictactoe.h"
#include "app/pong.h"
#include "app/metronome.h"
#include "app/delayer.h"

#include "lang/lang.h"

#define WIFI_NETWORK "Vodafone-284C30"
#define WIFI_PASS "tYUREqcuVn"

Touch touch;
Graphics graphics;
Lang lang;
Pins pins;
Notifications notifications;

//

App * App::APPS_REGISTRY[APP_REGISTRY_MAX_LENGTH];
size_t App::APPS_REGISTRY_LENGTH = 0;

//

App_Drawer drawer_app;
App_Statusbar statusbar_app;

App_Clock clock_app;
App_Monitor monitor_app;
App_Lang lang_app;
App_Metronome metronome_app;
App_weather weather_app;
App_Delayer delayer_app;

App_TicTacToe tictactoe_app;
App_Pong pong_app;

void wifi_task(void * pvParameters)  {
  while(true) {
    if (WiFi.status() == WL_CONNECTED) {
      vTaskDelay(10000 / portTICK_PERIOD_MS);
      continue;
    }

    Serial.printf("[WiFi] Connecting to WiFi\n");
    WiFi.begin(WIFI_NETWORK, WIFI_PASS);

    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime) < 20000){
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("[WiFi] Connected!");
      esp_Wsync_time();
      continue;
    }
  }
}

void setup() {
  Serial.begin(115200);

  Serial.println("[Main] Initializing SPIFFS");
  // check file system exists
  SPIFFS.begin(true);

  spi_mutex = xSemaphoreCreateMutex();
  Serial.println("[Main] Initializing TFT");
  tft.init();

  xTaskCreatePinnedToCore(
      wifi_task,
      "WiFi",
      4096,
      NULL,
      1,
      NULL,
      0
  );

  Serial.println("[Main] Calibration Touch");
  touch.calibrate();

  Serial.println("[Main] Calibration Touch");
  touch.calibrate();

  Serial.println("[Main] Initializing Touch");
  touch.begin();
  Serial.println("[Main] Initializing Graphics");
  graphics.begin();
  Serial.println("[Main] Initializing Pins");
  pins.begin();

  drawer_app.addApp(&clock_app);
  drawer_app.addApp(&monitor_app);
  drawer_app.addApp(&lang_app);
  drawer_app.addApp(&tictactoe_app);
  drawer_app.addApp(&metronome_app);
  drawer_app.addApp(&pong_app);
  drawer_app.addApp(&weather_app);
  drawer_app.addApp(&delayer_app);
  
  statusbar_app.open();
  drawer_app.open();

  vTaskDelay(500 / portTICK_PERIOD_MS);

  ledcSetup(4, 5000, 8);
  ledcAttachPin(27, 4);
  
  for(int i = 255; i >= 0; i -= 20) {
    ledcWrite(4, i);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }

  ledcWrite(4, 0);
}

void loop() {
  vTaskDelete(NULL);
}