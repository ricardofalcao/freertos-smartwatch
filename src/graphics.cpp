#include "graphics.h"
#include "tft.h"

#include <string.h>

#include "fonts/OpenSans-Regular6pt8b.h"
#include "fonts/OpenSans-Regular9pt8b.h"
#include "fonts/OpenSans-Regular12pt8b.h"
#include "fonts/OpenSans-Regular18pt8b.h"

const GFXfont * fonts[FONTS_LENGTH] PROGMEM = {
    &OpenSans_Regular6pt8b,
    &OpenSans_Regular9pt8b,
    &OpenSans_Regular12pt8b,
    &OpenSans_Regular18pt8b
};

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
    int32_t width;
    int32_t height;
    int32_t radius;
    uint32_t color;
    uint8_t thickness;

} RoundedRectangle_t;

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
    uint32_t fill;
    uint8_t font_size;

} String_t;

typedef struct {

    TFT_eSprite * sprite;
    int32_t x;
    int32_t y;

} Image_t;

typedef struct {

    uint32_t color;

} Screen_t;

typedef struct {

    size_t batch_size;

    GViewport_t viewport;

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
    operation_queue = xQueueCreate(GRAPHICS_OPERATION_QUEUE_SIZE, sizeof(Batch_t));
}

void Graphics::begin() {
    xTaskCreatePinnedToCore(
      graphics_task,
      "Graphics",
      10240 * 4,
      this,
      tskIDLE_PRIORITY + 3,
      NULL,
      0
  );
}

void Graphics::onTick() {
    Batch_t receive_buffer;

    if (xQueueReceive(operation_queue, &receive_buffer, portMAX_DELAY)) {
        GOperation_t * operations = (GOperation_t *) receive_buffer.pvData;

        if (xSemaphoreTake(spi_mutex, portMAX_DELAY) == pdTRUE) {
            tft.startWrite();
            tft.setViewport(receive_buffer.viewport.x, receive_buffer.viewport.y, receive_buffer.viewport.width, receive_buffer.viewport.height);

            for(size_t i = 0; i < receive_buffer.batch_size; i++) {
                processOperation(&operations[i]);
            }
            tft.endWrite();
            xSemaphoreGive(spi_mutex);

        }
        
        vPortFree(operations);
    }
}

void _draw_thick_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t wd, uint32_t color) {
    int32_t dx = abs(x1-x0), sx = x0 < x1 ? 1 : -1; 
    int32_t dy = abs(y1-y0), sy = y0 < y1 ? 1 : -1; 
    int32_t err = dx-dy, e2, x2, y2;

    float ed = dx+dy == 0 ? 1 : sqrt((float)dx*dx+(float)dy*dy);
    
    for (wd = (wd+1)/2; ; ) {                                   /* pixel loop */
        tft.drawPixel(x0,y0, color);
        e2 = err; x2 = x0;

        if (2*e2 >= -dx) {                                           /* x step */
            for (e2 += dy, y2 = y0; e2 < ed*wd && (y1 != y2 || dx > dy); e2 += dx)
                tft.drawPixel(x0, y2 += sy, color);
            if (x0 == x1) break;
            e2 = err; err -= dy; x0 += sx; 
        } 
        if (2*e2 <= dy) {                                            /* y step */
            for (e2 = dx-e2; e2 < ed*wd && (x1 != x2 || dx < dy); e2 += dy)
                tft.drawPixel(x2 += sx, y0, color);
            if (y0 == y1) break;
            err += dx; y0 += sy; 
        }
    }
}

void _xLine(int32_t x1, int32_t x2, int32_t y, uint32_t color)
{
    int32_t dx = tft.getViewportX();
    int32_t dy = tft.getViewportY();
    tft.setWindow(dx + x1, dy + y, dx + x2, dy + y);
    tft.pushBlock(color, x2 - x1 + 1);
}

void _yLine(int32_t x, int32_t y1, int32_t y2, uint32_t color)
{
    int32_t dx = tft.getViewportX();
    int32_t dy = tft.getViewportY();
    tft.setWindow(dx + x, dy + y1, dx + x, dy + y2);
    tft.pushBlock(color, y2 - y1 + 1);
}

void _draw_thick_circle(int32_t xc, int32_t yc, int32_t radius, uint8_t thickness, uint32_t color)
{
    int32_t outer = radius + thickness / 2;
    int32_t inner = radius - thickness / 2;

    int xo = outer;
    int xi = inner;
    int y = 0;
    int erro = 1 - xo;
    int erri = 1 - xi;

    while(xo >= y) {
        _xLine(xc + xi, xc + xo, yc + y,  color);
        _yLine(xc + y,  yc + xi, yc + xo, color);
        _xLine(xc - xo, xc - xi, yc + y,  color);
        _yLine(xc - y,  yc + xi, yc + xo, color);
        _xLine(xc - xo, xc - xi, yc - y,  color);
        _yLine(xc - y,  yc - xo, yc - xi, color);
        _xLine(xc + xi, xc + xo, yc - y,  color);
        _yLine(xc + y,  yc - xo, yc - xi, color);

        y++;

        if (erro < 0) {
            erro += 2 * y + 1;
        } else {
            xo--;
            erro += 2 * (y - xo) + 1;
        }

        if (y > inner) {
            xi = y;
        } else {
            if (erri < 0) {
                erri += 2 * y + 1;
            } else {
                xi--;
                erri += 2 * (y - xi) + 1;
            }
        }
    }
}

void Graphics::processOperation(GOperation_t * operation) {
    switch(operation->type) {
        case DRAW_RECTANGLE: {
            Rectangle_t * rect = (Rectangle_t *) operation->pvData;
            
            _draw_thick_line(rect->x, rect->y, rect->x + rect->width - 1, rect->y, rect->thickness, rect->color);
            _draw_thick_line(rect->x, rect->y, rect->x, rect->y + rect->height - 1, rect->thickness, rect->color);
            _draw_thick_line(rect->x, rect->y + rect->height - 1, rect->x + rect->width - 1, rect->y + rect->height - 1, rect->thickness, rect->color);
            _draw_thick_line(rect->x + rect->width - 1, rect->y, rect->x + rect->width - 1, rect->y + rect->height - 1, rect->thickness, rect->color);

            break;
        }

        case FILL_RECTANGLE: {
            Rectangle_t * rect = (Rectangle_t *) operation->pvData;
            tft.fillRect(rect->x, rect->y, rect->width, rect->height, rect->color);
            break;
        }

        //
        case DRAW_ROUNDED_RECTANGLE: {
            RoundedRectangle_t * rect = (RoundedRectangle_t *) operation->pvData;
            
            _draw_thick_line(rect->x, rect->y, rect->x + rect->width - 1, rect->y, rect->thickness, rect->color);
            _draw_thick_line(rect->x, rect->y, rect->x, rect->y + rect->height - 1, rect->thickness, rect->color);
            _draw_thick_line(rect->x, rect->y + rect->height - 1, rect->x + rect->width - 1, rect->y + rect->height - 1, rect->thickness, rect->color);
            _draw_thick_line(rect->x + rect->width - 1, rect->y, rect->x + rect->width - 1, rect->y + rect->height - 1, rect->thickness, rect->color);

            break;
        }

        case FILL_ROUNDED_RECTANGLE: {
            RoundedRectangle_t * rect = (RoundedRectangle_t *) operation->pvData;
            tft.fillRoundRect(rect->x, rect->y, rect->width, rect->height, rect->radius, rect->color);
            break;
        }

        //

        case DRAW_CIRCLE: {
            Circle_t * circle = (Circle_t *) operation->pvData;

            _draw_thick_circle(circle->x, circle->y, circle->radius, circle->thickness, circle->color);

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
            
            _draw_thick_line(tri->x1, tri->y1, tri->x2, tri->y2, tri->thickness, tri->color);
            _draw_thick_line(tri->x2, tri->y2, tri->x3, tri->y3, tri->thickness, tri->color);
            _draw_thick_line(tri->x3, tri->y3, tri->x1, tri->y1, tri->thickness, tri->color);
            
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
            
            _draw_thick_line(line->xstart, line->ystart, line->xend, line->yend, line->thickness, line->color);

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
                tft.setFreeFont(text->font_size <= 0 || text->font_size > FONTS_LENGTH ? fonts[0] : fonts[text->font_size - 1]);
                tft.setTextSize(1);

                tft.setTextColor(text->color, text->fill);
                tft.setTextDatum(text->datum);
                tft.drawString(text->str, text->x, text->y);

                vPortFree(text->str);
            }

            break;
        }

        case DRAW_IMAGE: {
            Image_t * image = (Image_t *) operation->pvData;
            image->sprite->pushSprite(image->x, image->y);
            break;
        }

        case FILL_SCREEN: {
            Screen_t * screen = (Screen_t *) operation->pvData;
            tft.fillScreen(screen->color);
            break;
        }
    }

    vPortFree(operation->pvData);
}

/*

*/

GBatch_t::GBatch_t(GViewport_t _viewport) {
    viewport = _viewport;
}

GBatch_t Graphics::beginBatch(GViewport_t _viewport) {
    return GBatch_t(_viewport);
}

void Graphics::endBatch(GBatch_t * _batch) {
    if (_batch->batch_queue_length == 0) {
        return;
    }

    GOperation_t * copy = (GOperation_t *) pvPortMalloc(sizeof(GOperation_t) * _batch->batch_queue_length);

    for(size_t i = 0; i < _batch->batch_queue_length; i++) {
        copy[i] = _batch->batch_queue[i];
    }

    Batch_t batch = {
        .batch_size = _batch->batch_queue_length,
        .viewport = _batch->viewport,
        .pvData = (void *) copy 
    };

    xQueueSendToBack(operation_queue, &batch, portMAX_DELAY);
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

RoundedRectangle_t * _rounded_rectangle(int32_t x, int32_t y, int32_t width, int32_t height, int32_t radius, uint32_t color, uint8_t thickness) {
    RoundedRectangle_t * rectangle = (RoundedRectangle_t *) pvPortMalloc(sizeof(RoundedRectangle_t));
    rectangle->x = x;
    rectangle->y = y;
    rectangle->width = width;
    rectangle->height = height;
    rectangle->radius = radius;
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

String_t * _text(int32_t x, int32_t y, const char * str, uint8_t datum, uint32_t color, uint32_t fill, uint8_t font_size) {
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
    text->fill = fill;
    text->font_size = font_size;
    return text;
}

Image_t * _image(TFT_eSprite * sprite, int32_t x, int32_t y) {
    Image_t * image = (Image_t *) pvPortMalloc(sizeof(Image_t));
    image->sprite = sprite;
    image->x = x;
    image->y = y;
    return image;
}

Screen_t * _screen(uint32_t color) {
    Screen_t * screen = (Screen_t *) pvPortMalloc(sizeof(Screen_t));
    screen->color = color;
    return screen;
}

//

void GBatch_t::drawRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color, uint8_t thickness) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = DRAW_RECTANGLE;
    operation->pvData = (void *) _rectangle(x, y, width, height, color, thickness);
}

void GBatch_t::fillRectangle(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = FILL_RECTANGLE;
    
    operation->pvData = (void *) _rectangle(x, y, width, height, color, 0);

    
}

//

void GBatch_t::drawRoundedRectangle(int32_t x, int32_t y, int32_t width, int32_t height, int32_t radius, uint32_t color, uint8_t thickness) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = DRAW_ROUNDED_RECTANGLE;
    operation->pvData = (void *) _rounded_rectangle(x, y, width, height, radius, color, thickness);
}

void GBatch_t::fillRoundedRectangle(int32_t x, int32_t y, int32_t width, int32_t height, int32_t radius, uint32_t color) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = FILL_ROUNDED_RECTANGLE;
    operation->pvData = (void *) _rounded_rectangle(x, y, width, height, radius, color, 0);
}

//

void GBatch_t::drawCircle(int32_t x, int32_t y, int32_t radius, uint32_t color, uint8_t thickness) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = DRAW_CIRCLE;
    operation->pvData = (void *) _circle(x, y, radius, color, thickness);
}

void GBatch_t::fillCircle(int32_t x, int32_t y, int32_t radius, uint32_t color) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = FILL_CIRCLE;
    operation->pvData = (void *) _circle(x, y, radius, color, 0);    
}

//

void GBatch_t::drawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color, uint8_t thickness) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = DRAW_TRIANGLE;
    operation->pvData = (void *) _triangle(x1, y1, x2, y2, x3, y3, color, thickness);
}

void GBatch_t::fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint32_t color) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = FILL_TRIANGLE;
    operation->pvData = (void *) _triangle(x1, y1, x2, y2, x3, y3, color, 0);
}

void GBatch_t::drawLine(int32_t xstart, int32_t ystart, int32_t xend, int32_t yend, uint32_t color, uint8_t thickness) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = DRAW_LINE;
    operation->pvData = (void *) _line(xstart, ystart, xend, yend, color, thickness);
}

void GBatch_t::drawPixel(int32_t x, int32_t y, uint32_t color) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = DRAW_PIXEL;
    operation->pvData = (void *) _pixel(x, y, color);
}

void GBatch_t::drawString(int32_t x, int32_t y, const char * string, uint32_t color, uint8_t font_size, uint8_t datum) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = DRAW_STRING;    
    operation->pvData = (void *) _text(x, y, string, datum, color, color, font_size);    
}

void GBatch_t::drawFilledString(int32_t x, int32_t y, const char * string, uint32_t color, uint32_t fill, uint8_t font_size, uint8_t datum) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = DRAW_STRING;    
    operation->pvData = (void *) _text(x, y, string, datum, color, fill, font_size);    
}

void GBatch_t::drawImage(TFT_eSprite * sprite, int32_t x, int32_t y) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = DRAW_IMAGE;    
    operation->pvData = (void *) _image(sprite, x, y);    
}

void GBatch_t::fillScreen(uint32_t color) {
    GOperation_t * operation = &batch_queue[batch_queue_length ++];
    
    operation->type = FILL_SCREEN;    
    operation->pvData = (void *) _screen(color);    
}

int32_t GBatch_t::viewHeight() {
    return viewport.height;
}

int32_t GBatch_t::viewWidth() {
    return viewport.width;
}