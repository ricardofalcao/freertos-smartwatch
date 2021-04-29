#include "graphics.h"
#include "tft.h"

#include <string.h>

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

typedef struct {

    int32_t x;
    int32_t y;
    char * str;
    uint8_t datum;
    uint32_t color;
    uint8_t font_size;

} String_t;

typedef struct {

    uint32_t color;

} Screen_t;

typedef struct {

    size_t batch_size;

    void * pvData;

} Batch_t;

/*

*/

void graphics_task(void * pvParameters) {
    Graphics * g = (Graphics *) pvParameters;
    while(true) {
        g->onTick();
    }
}

/*

*/

Graphics::Graphics() {
    operation_queue = xQueueCreate(GRAPHICS_OPERATION_QUEUE_SIZE, sizeof(GOperation_t));
}

void Graphics::begin() {
    xTaskCreatePinnedToCore(
      graphics_task,
      "Graphics",
      10240,
      this,
      1,
      NULL,
      0
  );
}

void Graphics::setViewport(GViewport_t viewport) {
    viewport_buffer = viewport;
}

void Graphics::onTick() {
    GOperation_t receive_buffer;
    if (xQueueReceive(operation_queue, &receive_buffer, portMAX_DELAY)) {
        if (xSemaphoreTake(spi_mutex, portMAX_DELAY) == pdTRUE) {
            processOperation(&receive_buffer);
            xSemaphoreGive(spi_mutex);
        }
    }
}

void Graphics::processOperation(GOperation_t * operation) {
    tft.setViewport(operation->viewport.x, operation->viewport.y, operation->viewport.width, operation->viewport.height);

    switch(operation->type) {
        case DRAW_RECTANGLE: {
            Rectangle_t * rect = (Rectangle_t *) operation->pvData;
            
            for(int8_t i = -rect->thickness / 2; i < rect->thickness / 2; i++) {
                tft.drawRect(rect->x + i, rect->y + i, rect->width - i, rect->height - i, rect->color);
            }

            break;
        }

        case FILL_RECTANGLE: {
            Rectangle_t * rect = (Rectangle_t *) operation->pvData;
            tft.fillRect(rect->x, rect->y, rect->width, rect->height, rect->color);
            break;
        }

        //

        case DRAW_CIRCLE: {
            Circle_t * circle = (Circle_t *) operation->pvData;

            for(int8_t i = -circle->thickness / 2; i < circle->thickness / 2 + circle->thickness % 2; i++) {
                tft.drawCircle(circle->x, circle->y, circle->radius + i, circle->color);
            }

            break;
        }

        case FILL_CIRCLE: {
            Circle_t * circle = (Circle_t *) operation->pvData;
            tft.fillCircle(circle->x, circle->y, circle->radius, circle->color);
            break;
        }

        //

        case DRAW_TRIANGLE: {
            Triangle_t * tri = (Triangle_t *) operation->pvData;
            tft.drawTriangle(tri->x1, tri->y1, tri->x2, tri->y2, tri->x3, tri->y3, tri->color);
            break;
        }

        case FILL_TRIANGLE: {
            Triangle_t * tri = (Triangle_t *) operation->pvData;
            tft.fillTriangle(tri->x1, tri->y1, tri->x2, tri->y2, tri->x3, tri->y3, tri->color);
            break;
        }

        //

        case DRAW_LINE: {
            Line_t * line = (Line_t *) operation->pvData;
            int32_t dx = line->xend - line->xstart;
            int32_t dy = line->yend - line->ystart;

            float mag = sqrt(dx*dx + dy*dy);

            float p_dx = dy / mag;
            float p_dy = -dx / mag;

            for(int8_t i = -line->thickness / 2; i < line->thickness / 2; i++) {
                tft.drawLine(line->xstart + p_dx*i, line->ystart + p_dy*i, line->xend + p_dx*i, line->yend + p_dy*i, line->color);
            }

            break;
        }

        case DRAW_PIXEL: {
            Pixel_t * pixel = (Pixel_t *) operation->pvData;
            tft.drawPixel(pixel->x, pixel->y, pixel->color);
            break;
        }

        case DRAW_STRING: {
            String_t * text = (String_t *) operation->pvData;

            if (text->str) {
                //tft.loadFont("NotoSans-Regular20");
                tft.setTextColor(text->color);
                tft.setTextSize(text->font_size);
                tft.setTextDatum(text->datum);
                tft.drawString(text->str, text->x, text->y);
                //tft.unloadFont();

                vPortFree(text->str);
            }

            break;
        }

        case FILL_SCREEN: {
            Screen_t * screen = (Screen_t *) operation->pvData;
            tft.fillScreen(screen->color);
            break;
        }

        case BATCH: {
            Batch_t * batch = (Batch_t *) operation->pvData;
            GOperation_t * operations = (GOperation_t *) batch->pvData;

            tft.startWrite();
            for(size_t i = 0; i < batch->batch_size; i++) {
                processOperation(&operations[i]);
            }
            tft.endWrite();

            vPortFree(operations);
            break;
        }
    }

    vPortFree(operation->pvData);
}

/*

*/

void Graphics::beginBatch() {
    batching = true;
    batch_queue_length = 0;
}

void Graphics::endBatch() {
    if(!batching) {
        return;
    }

    batching = false;

    Batch_t * batch = (Batch_t *) pvPortMalloc(sizeof(Batch_t));
    batch->batch_size = batch_queue_length;

    GOperation_t * copy = (GOperation_t *) pvPortMalloc(sizeof(GOperation_t) * batch_queue_length);

    for(size_t i = 0; i < batch_queue_length; i++) {
        copy[i] = batch_queue[i];
    }

    batch->pvData = (void *) copy;

    const GOperation_t operation = {
        .type = BATCH,
        .viewport = {},
        .pvData = (void *) batch
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

/*

*/

Rectangle_t * _rectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color, uint8_t thickness) {
    Rectangle_t * rectangle = (Rectangle_t *) pvPortMalloc(sizeof(Rectangle_t));
    rectangle->x = x;
    rectangle->y = y;
    rectangle->width = width;
    rectangle->height = height;
    rectangle->color = color;
    rectangle->thickness = thickness;
    return rectangle;
}

Circle_t * _circle(int32_t x, int32_t y, int32_t radius, uint32_t color, uint8_t thickness) {
    Circle_t * circle = (Circle_t *) pvPortMalloc(sizeof(Circle_t));
    circle->x = x;
    circle->y = y;
    circle->radius = radius;
    circle->color = color;
    circle->thickness = thickness;
    return circle;
}

Triangle_t * _triangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color, uint8_t thickness) {
    Triangle_t * triangle = (Triangle_t *) pvPortMalloc(sizeof(Triangle_t));
    triangle->x1 = x1;
    triangle->y1 = y1;
    triangle->x2 = x2;
    triangle->y2 = y2;
    triangle->x3 = x3;
    triangle->y3 = y3;
    triangle->color = color;
    triangle->thickness = thickness;
    return triangle;
}

Line_t * _line(int32_t xstart, int32_t ystart, int32_t xend, int32_t yend, uint32_t color, uint8_t thickness) {
    Line_t * line = (Line_t *) pvPortMalloc(sizeof(Line_t));
    line->xstart = xstart;
    line->ystart = ystart;
    line->xend = xend;
    line->yend = yend;
    line->color = color;
    line->thickness = thickness;
    return line;
}

Pixel_t * _pixel(int32_t x, int32_t y, uint32_t color) {
    Pixel_t * pixel = (Pixel_t *) pvPortMalloc(sizeof(Pixel_t));
    pixel->x = x;
    pixel->y = y;
    pixel->color = color;
    return pixel;
}

String_t * _text(int32_t x, int32_t y, const char * str, uint8_t datum, uint32_t color, uint8_t font_size) {
    size_t len = strlen(str) + 1;
    char * copy = (char *) pvPortMalloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }

    String_t * text = (String_t *) pvPortMalloc(sizeof(String_t));
    text->x = x;
    text->y = y;
    text->str = copy;
    text->datum = datum;
    text->color = color;
    text->font_size = font_size;
    return text;
}

Screen_t * _screen(uint32_t color) {
    Screen_t * screen = (Screen_t *) pvPortMalloc(sizeof(Screen_t));
    screen->color = color;
    return screen;
}

//

GOperation_t * Graphics::getOperationBuffer() {
    if (batching) {
        return &batch_queue[batch_queue_length];
    }

    return &operation_buffer;
}

void Graphics::enqueueOperationBuffer() {
    if (batching) {
        batch_queue_length++;
        return;
    }

    xQueueSendToBack(operation_queue, &operation_buffer, portMAX_DELAY);
}

void Graphics::drawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color, uint8_t thickness) {
    
    GOperation_t * operation = getOperationBuffer();
    
    operation->type = DRAW_RECTANGLE;
    operation->viewport = viewport_buffer;
    operation->pvData = (void *) _rectangle(x, y, width, height, color, thickness);

    enqueueOperationBuffer();
}

void Graphics::fillRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {
    GOperation_t * operation = getOperationBuffer();
    
    operation->type = FILL_RECTANGLE;
    operation->viewport = viewport_buffer;
    operation->pvData = (void *) _rectangle(x, y, width, height, color, 0);

    enqueueOperationBuffer();
}

//

void Graphics::drawCircle(int32_t x, int32_t y, int32_t radius, uint32_t color, uint8_t thickness) {
    GOperation_t * operation = getOperationBuffer();
    
    operation->type = DRAW_CIRCLE;
    operation->viewport = viewport_buffer;
    operation->pvData = (void *) _circle(x, y, radius, color, thickness);

    enqueueOperationBuffer();
}

void Graphics::fillCircle(int32_t x, int32_t y, int32_t radius, uint32_t color) {
    GOperation_t * operation = getOperationBuffer();
    
    operation->type = FILL_CIRCLE;
    operation->viewport = viewport_buffer;
    operation->pvData = (void *) _circle(x, y, radius, color, 0);

    enqueueOperationBuffer();
}

//

void Graphics::drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color, uint8_t thickness) {
    GOperation_t * operation = getOperationBuffer();
    
    operation->type = DRAW_TRIANGLE;
    operation->viewport = viewport_buffer;
    operation->pvData = (void *) _triangle(x1, y1, x2, y2, x3, y3, color, thickness);

    enqueueOperationBuffer();
}

void Graphics::fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color) {
    GOperation_t * operation = getOperationBuffer();
    
    operation->type = FILL_TRIANGLE;
    operation->viewport = viewport_buffer;
    operation->pvData = (void *) _triangle(x1, y1, x2, y2, x3, y3, color, 0);

    enqueueOperationBuffer();
}

void Graphics::drawLine(int32_t xstart, int32_t ystart, int32_t xend, int32_t yend, uint32_t color, uint8_t thickness) {
    GOperation_t * operation = getOperationBuffer();
    
    operation->type = DRAW_LINE;
    operation->viewport = viewport_buffer;
    operation->pvData = (void *) _line(xstart, ystart, xend, yend, color, thickness);

    enqueueOperationBuffer();
}

void Graphics::drawPixel(int32_t x, int32_t y, uint32_t color) {
    GOperation_t * operation = getOperationBuffer();
    
    operation->type = DRAW_PIXEL;
    operation->viewport = viewport_buffer;
    operation->pvData = (void *) _pixel(x, y, color);

    enqueueOperationBuffer();
}

void Graphics::drawString(int32_t x, int32_t y, const char * string, uint32_t color, uint8_t font_size, uint8_t datum) {
    GOperation_t * operation = getOperationBuffer();
    
    operation->type = DRAW_STRING;
    operation->viewport = viewport_buffer;
    operation->pvData = (void *) _text(x, y, string, datum, color, font_size);

    enqueueOperationBuffer();
}

void Graphics::fillScreen(uint32_t color) {
    GOperation_t * operation = getOperationBuffer();
    
    operation->type = FILL_SCREEN;
    operation->viewport = viewport_buffer;
    operation->pvData = (void *) _screen(color);

    enqueueOperationBuffer();
}