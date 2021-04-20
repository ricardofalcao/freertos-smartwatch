#include "graphics.h"
#include "tft.h"

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
    const char * str;
    uint8_t datum;
    uint32_t color;
    uint8_t font_size;

} String_t;

typedef struct {

    uint32_t color;

} Screen_t;

/*

*/

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
    current_viewport = viewport;
}

void Graphics::onTick() {
    if (xQueueReceive(operation_queue, &receive_buffer, portMAX_DELAY)) {
        if (xSemaphoreTake(spi_mutex, portMAX_DELAY) == pdTRUE) {
            tft.setViewport(receive_buffer.viewport.x, receive_buffer.viewport.y, receive_buffer.viewport.width, receive_buffer.viewport.height);

            switch(receive_buffer.type) {
                case DRAW_RECTANGLE: {
                    Rectangle_t * rect = (Rectangle_t *) receive_buffer.pvData;
                    
                    for(int8_t i = -rect->thickness / 2; i < rect->thickness / 2; i++) {
                        tft.drawRect(rect->x + i, rect->y + i, rect->width - i, rect->height - i, rect->color);
                    }

                    Serial.printf("Drawing %d %d %d %d %d\n", rect->x, rect->y, rect->width, rect->height, rect->color);
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

                    for(int8_t i = -circle->thickness / 2; i < circle->thickness / 2 + circle->thickness % 2; i++) {
                        tft.drawCircle(circle->x, circle->y, circle->radius + i, circle->color);
                    }

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
                    Pixel_t * pixel = (Pixel_t *) receive_buffer.pvData;
                    tft.drawPixel(pixel->x, pixel->y, pixel->color);
                    break;
                }

                case DRAW_STRING: {
                    String_t * text = (String_t *) receive_buffer.pvData;

                    //tft.loadFont("NotoSans-Regular20");
                    tft.setTextColor(text->color);
                    tft.setTextSize(text->font_size);
                    tft.setTextDatum(text->datum);
                    tft.drawString(text->str, text->x, text->y);
                    //tft.unloadFont();

                    break;
                }

                case FILL_SCREEN: {
                    Screen_t * screen = (Screen_t *) receive_buffer.pvData;
                    tft.fillScreen(screen->color);
                    break;
                }
            }

            xSemaphoreGive(spi_mutex);
            vPortFree(receive_buffer.pvData);
        }
    }
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
    String_t * text = (String_t *) pvPortMalloc(sizeof(String_t));
    text->x = x;
    text->y = y;
    text->str = str;
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

void Graphics::drawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color, uint8_t thickness) {
    const GOperation_t operation = {
        .type = DRAW_RECTANGLE,
        .viewport = current_viewport,
        .pvData = (void *) _rectangle(x, y, width, height, color, thickness)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::fillRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {
    const GOperation_t operation = {
        .type = FILL_RECTANGLE,
        .viewport = current_viewport,
        .pvData = (void *) _rectangle(x, y, width, height, color, 0)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

//

void Graphics::drawCircle(int32_t x, int32_t y, int32_t radius, uint32_t color, uint8_t thickness) {
    const GOperation_t operation = {
        .type = DRAW_CIRCLE,
        .viewport = current_viewport,
        .pvData = (void *) _circle(x, y, radius, color, thickness)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::fillCircle(int32_t x, int32_t y, int32_t radius, uint32_t color) {
    const GOperation_t operation = {
        .type = FILL_CIRCLE,
        .viewport = current_viewport,
        .pvData = (void *) _circle(x, y, radius, color, 0)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

//

void Graphics::drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color, uint8_t thickness) {
    const GOperation_t operation = {
        .type = DRAW_TRIANGLE,
        .viewport = current_viewport,
        .pvData = (void *) _triangle(x1, y1, x2, y2, x3, y3, color, thickness)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color) {
    const GOperation_t operation = {
        .type = FILL_TRIANGLE,
        .viewport = current_viewport,
        .pvData = (void *) _triangle(x1, y1, x2, y2, x3, y3, color, 0)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::drawLine(int32_t xstart, int32_t ystart, int32_t xend, int32_t yend, uint32_t color, uint8_t thickness) {
    const GOperation_t operation = {
        .type = DRAW_LINE,
        .viewport = current_viewport,
        .pvData = (void *) _line(xstart, ystart, xend, yend, color, thickness)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::drawPixel(int32_t x, int32_t y, uint32_t color) {
    const GOperation_t operation = {
        .type = DRAW_PIXEL,
        .viewport = current_viewport,
        .pvData = (void *) _pixel(x, y, color)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::drawString(int32_t x, int32_t y, const char * string, uint32_t color, uint8_t font_size, uint8_t datum) {
    const GOperation_t operation = {
        .type = DRAW_STRING,
        .viewport = current_viewport,
        .pvData = (void *) _text(x, y, string, datum, color, font_size)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}

void Graphics::fillScreen(uint32_t color) {
    const GOperation_t operation = {
        .type = FILL_SCREEN,
        .viewport = current_viewport,
        .pvData = (void *) _screen(color)
    };

    xQueueSendToBack(operation_queue, &operation, portMAX_DELAY);
}