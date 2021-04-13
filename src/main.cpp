#include <Arduino.h>

#include "app/clock.h"

App_Clock clock_app;

unsigned long startTime;

void setup() {
  Serial.begin(115200);
  Serial.println("Hello World!");

  clock_app.open();

  startTime = millis();
}

void loop() {
  if ((millis() - startTime) > 10000) {
    if (clock_app.close()) {
      Serial.println("Close!!");
      startTime = millis();
    }
  }
}