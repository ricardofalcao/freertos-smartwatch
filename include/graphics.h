#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

#define GRAPHICS_OPERATION_QUEUE_SIZE 32

typedef enum {

    DRAW_RECTANGLE = 0,
    FILL_RECTANGLE,

    DRAW_CIRCLE,
    FILL_CIRCLE,

    DRAW_TRIANGLE,
    FILL_TRIANGLE,

    DRAW_LINE,
    DRAW_PIXEL

} GOperationType_t;

typedef struct {

    GOperationType_t type;

    void * pvData;

} GOperation_t;

/*

*/



typedef struct {

    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    uint32_t color;
    uint8_t thickness;

} Rectangle_t;

typedef struct {

    int32_t x;
    int32_t y;
    int32_t radius;
    uint32_t color;
    uint8_t thickness;

} Circle_t;

typedef struct {

    int32_t x1;
    int32_t y1;
    int32_t x2;
    int32_t y2;
    int32_t x3;
    int32_t y3;
    uint32_t color;
    uint8_t thickness;

} Triangle_t;

typedef struct {

    int32_t xstart;
    int32_t ystart;
    int32_t xend;
    int32_t yend;
    uint8_t thickness;
    uint32_t color;

} Line_t;

typedef struct {

    int32_t x;
    int32_t y;
    uint32_t color;

} Pixel_t;

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

        void drawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color, uint8_t thickness = 1);

        void fillRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color);
        
        //

        void drawCircle(int32_t x, int32_t y, int32_t radius, uint32_t color, uint8_t thickness = 1);

        void fillCircle(int32_t x, int32_t y, int32_t radius, uint32_t color);
        
        //

        void drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color, uint8_t thickness = 1);

        void fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color);

        //

        void drawLine(int32_t xstart, int32_t ystart, int32_t xend, int32_t yend, uint32_t color, uint8_t thickness = 1);

        void drawPixel(int32_t x, int32_t y, uint32_t color);

};

extern Graphics graphics;