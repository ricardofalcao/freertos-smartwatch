#pragma once
#include "sdkconfig.h"

#include <Arduino.h>
#include <TFT_eSPI.h>

#define GRAPHICS_OPERATION_QUEUE_SIZE   16
#define GRAPHICS_BATCH_MAX_SIZE         32

#define FONTS_LENGTH 4

extern const GFXfont * fonts[FONTS_LENGTH] PROGMEM;

typedef struct {

    int32_t x;
    
    int32_t y;
    
    int32_t width;

    int32_t height;

} GViewport_t;

/*

*/

typedef enum {

    DRAW_RECTANGLE = 0,
    FILL_RECTANGLE,

    DRAW_ROUNDED_RECTANGLE,
    FILL_ROUNDED_RECTANGLE,

    DRAW_CIRCLE,
    FILL_CIRCLE,

    DRAW_TRIANGLE,
    FILL_TRIANGLE,

    DRAW_LINE,
    DRAW_PIXEL,

    DRAW_STRING,
    DRAW_IMAGE,

    FILL_SCREEN,

} GOperationType_t;

typedef struct {

    GOperationType_t type;

    void * pvData;

} GOperation_t;

/*

*/

class GBatch_t {
    public:
        GViewport_t viewport;

        GOperation_t batch_queue[GRAPHICS_BATCH_MAX_SIZE];
        size_t batch_queue_length = 0;

    public:
        GBatch_t(GViewport_t viewport);

        void drawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color, uint8_t thickness = 1);

        void fillRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color);

        //

        void drawRoundedRectangle(int32_t x, int32_t y, int32_t width, int32_t height, int32_t radius, uint32_t color, uint8_t thickness = 1);

        void fillRoundedRectangle(int32_t x, int32_t y, int32_t width, int32_t height, int32_t radius, uint32_t color);
        
        //

        void drawCircle(int32_t x, int32_t y, int32_t radius, uint32_t color, uint8_t thickness = 1);

        void fillCircle(int32_t x, int32_t y, int32_t radius, uint32_t color);
        
        //

        void drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color, uint8_t thickness = 1);

        void fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color);

        //

        void drawLine(int32_t xstart, int32_t ystart, int32_t xend, int32_t yend, uint32_t color, uint8_t thickness = 1);

        void drawPixel(int32_t x, int32_t y, uint32_t color);

        //

        void drawString(int32_t x, int32_t y, const char * string, uint32_t color, uint8_t font_size = 4, uint8_t datum = TL_DATUM);

        void drawFilledString(int32_t x, int32_t y, const char * string, uint32_t color, uint32_t fill, uint8_t font_size = 4, uint8_t datum = TL_DATUM);

        //

        void drawImage(TFT_eSprite * sprite, int32_t x, int32_t y);

        //

        void fillScreen(uint32_t color);

        int32_t viewHeight();

        int32_t viewWidth();
};

/*

*/

class Graphics {
    private:
        QueueHandle_t operation_queue;

    public:
        Graphics();

        void begin();

        void onTick();

        //

        GBatch_t beginBatch(GViewport_t _viewport);

        void endBatch(GBatch_t * batch);

    private:
        void processOperation(GOperation_t * operation);

};

extern Graphics graphics;