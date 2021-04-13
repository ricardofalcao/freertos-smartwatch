#include <Arduino.h>

#include <SPI.h>
#include <TFT_eSPI.h>

#include "graphics.h"
#include "app/clock.h"
#include "app/alert.h"

Graphics graphics;

App_Clock clock_app;
App_Alert alert_app;

unsigned long startTime;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!");

  graphics.begin();

  clock_app.open();
  //alert_app.open();

  startTime = millis();
}

void loop() {
  if ((millis() - startTime) > 10000) {
    if (clock_app.running) {
      Serial.println("Close!!");
      clock_app.close();
      
      startTime = millis();
    }
  }
}