#include "graphics.h"

GOperation_t receive_buffer;

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
    
    tft.init();
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

void Graphics::onTick() {
    if (xQueueReceive(operation_queue, &receive_buffer, portMAX_DELAY)) {
        switch(receive_buffer.type) {
            case DRAW_RECTANGLE: {
                Rectangle_t * rect = (Rectangle_t *) receive_buffer.pvData;
                tft.drawRect(rect->x, rect->y, rect->width, rect->height, rect->color);
                break;
            }

            case FILL_RECTANGLE: {
                Rectangle_t * rect = (Rectangle_t *) receive_buffer.pvData;
                tft.fillRect(rect->x, rect->y, rect->width, rect->height, rect->color);
                break;
            }

            //

            case DRAW_CIRCLE: {
                Circle_t * circle = (Circle_t *) receive_buffer.pvData;
                tft.drawCircle(circle->x, circle->y, circle->radius, circle->color);
                break;
            }

            case FILL_CIRCLE: {
                Circle_t * circle = (Circle_t *) receive_buffer.pvData;
                tft.fillCircle(circle->x, circle->y, circle->radius, circle->color);
                break;
            }

            //

            case DRAW_TRIANGLE: {
                Triangle_t * tri = (Triangle_t *) receive_buffer.pvData;
                tft.drawTriangle(tri->x1, tri->y1, tri->x2, tri->y2, tri->x3, tri->y3, tri->color);
                break;
            }

            case FILL_TRIANGLE: {
                Triangle_t * tri = (Triangle_t *) receive_buffer.pvData;
                tft.fillTriangle(tri->x1, tri->y1, tri->x2, tri->y2, tri->x3, tri->y3, tri->color);
                break;
            }

            //

            case DRAW_LINE: {
                Line_t * line = (Line_t *) receive_buffer.pvData;
                tft.drawLine(line->xstart, line->ystart, line->xend, line->yend, line->color);
                break;
            }

            case DRAW_PIXEL: {
                Pixel_t * pixel = (Pixel_t *) receive_buffer.pvData;
                tft.drawPixel(pixel->x, pixel->y, pixel->color);
                break;
            }


        }

        vPortFree(receive_buffer.pvData);
    }
}

/*

*/

Rectangle_t * _rectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {
    Rectangle_t * rectangle = (Rectangle_t *) pvPortMalloc(sizeof(Rectangle_t));
    rectangle->x = x;
    rectangle->y = y;
    rectangle->width = width;
    rectangle->height = height;
    rectangle->color = color;
    return rectangle;
}

Circle_t * _circle(int32_t x, int32_t y, int32_t radius, uint32_t color) {
    Circle_t * circle = (Circle_t *) pvPortMalloc(sizeof(Circle_t));
    circle->x = x;
    circle->y = y;
    circle->radius = radius;
    circle->color = color;
    return circle;
}

Triangle_t * _triangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color) {
    Triangle_t * triangle = (Triangle_t *) pvPortMalloc(sizeof(Triangle_t));
    triangle->x1 = x1;
    triangle->y1 = y1;
    triangle->x2 = x2;
    triangle->y2 = y2;
    triangle->x3 = x3;
    triangle->y3 = y3;
    triangle->color = color;
    return triangle;
}

Line_t * _line(int32_t xstart, int32_t ystart, int32_t xend, int32_t yend, uint32_t color) {
    Line_t * line = (Line_t *) pvPortMalloc(sizeof(Line_t));
    line->xstart = xstart;
    line->ystart = ystart;
    line->xend = xend;
    line->yend = yend;
    line->color = color;
    return line;
}

Pixel_t * _pixel(int32_t x, int32_t y, uint32_t color) {
    Pixel_t * pixel = (Pixel_t *) pvPortMalloc(sizeof(Pixel_t));
    pixel->x = x;
    pixel->y = y;
    pixel->color = color;
    return pixel;
}

//

void Graphics::drawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {
    const GOperation_t operation = {
        .type = DRAW_RECTANGLE,
        .pvData = (void *) _rectangle(x, y, width, height, color)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::fillRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {
    const GOperation_t operation = {
        .type = FILL_RECTANGLE,
        .pvData = (void *) _rectangle(x, y, width, height, color)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

//

void Graphics::drawCircle(int32_t x, int32_t y, int32_t radius, uint32_t color) {
    const GOperation_t operation = {
        .type = DRAW_CIRCLE,
        .pvData = (void *) _circle(x, y, radius, color)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::fillCircle(int32_t x, int32_t y, int32_t radius, uint32_t color) {
    const GOperation_t operation = {
        .type = FILL_CIRCLE,
        .pvData = (void *) _circle(x, y, radius, color)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

//

void Graphics::drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color) {
    const GOperation_t operation = {
        .type = DRAW_TRIANGLE,
        .pvData = (void *) _triangle(x1, y1, x2, y2, x3, y3, color)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color) {
    const GOperation_t operation = {
        .type = FILL_TRIANGLE,
        .pvData = (void *) _triangle(x1, y1, x2, y2, x3, y3, color)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::drawLine(int32_t xstart, int32_t ystart, int32_t xend, int32_t yend, uint32_t color) {
    const GOperation_t operation = {
        .type = DRAW_LINE,
        .pvData = (void *) _line(xstart, ystart, xend, yend, color)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::drawPixel(int32_t x, int32_t y, uint32_t color) {
    const GOperation_t operation = {
        .type = DRAW_PIXEL,
        .pvData = (void *) _pixel(x, y, color)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}