#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;

extern SemaphoreHandle_t spi_mutex;