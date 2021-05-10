#include "app/drawer.h"

#include "tft.h"
#include "touch.h"
#include "graphics.h"
#include "pins.h"

#define APP_WIDTH           ((VIEW_WIDTH - 2*APPS_MARGIN_X - APPS_SPACING_X * (DRAWER_COLUMNS - 1)) / DRAWER_COLUMNS)
#define APP_HEIGHT          APP_WIDTH
#define APP_BORDER_RADIUS   (APP_WIDTH / 4)

#define PAGE_ARROW_HEIGHT   (int) (0.8660254037844386f * PAGE_ARROW_SIZE)
#define PAGE_ARROW_CLICK_PADDING    5

App_Drawer::App_Drawer() : App("Drawer", "Shows all installed apps") {
    priority = 2;
    stack_depth = 10240;
    touch_stack_depth = 4096;

    queue_set = xQueueCreateSet(1 + 1 + 1);

    app_open_queue = xQueueCreate(1, sizeof(App *));
    redraw_signal = xSemaphoreCreateBinary();

    xQueueAddToSet(app_open_queue, queue_set);
    xQueueAddToSet(redraw_signal, queue_set);
    xQueueAddToSet(pins.gpio0, queue_set);
}

void App_Drawer::addApp(App * app) {
    apps[apps_length++] = app;
}

void App_Drawer::click_home() {

}

void App_Drawer::onOpen() {
    previous_page_touch_listener.x = PAGE_ARROW_MARGIN_X - PAGE_ARROW_CLICK_PADDING;
    previous_page_touch_listener.y = VIEW_HEIGHT - PAGE_ARROW_MARGIN_Y - PAGE_ARROW_SIZE / 2 - PAGE_ARROW_CLICK_PADDING;
    previous_page_touch_listener.width = PAGE_ARROW_HEIGHT + 2*PAGE_ARROW_CLICK_PADDING;
    previous_page_touch_listener.height = PAGE_ARROW_SIZE + 2*PAGE_ARROW_CLICK_PADDING;

    next_page_touch_listener.x = VIEW_WIDTH - PAGE_ARROW_MARGIN_X - PAGE_ARROW_HEIGHT - PAGE_ARROW_CLICK_PADDING;
    next_page_touch_listener.y = VIEW_HEIGHT - PAGE_ARROW_MARGIN_Y - PAGE_ARROW_SIZE / 2 - PAGE_ARROW_CLICK_PADDING;
    next_page_touch_listener.width = PAGE_ARROW_HEIGHT + 2*PAGE_ARROW_CLICK_PADDING;
    next_page_touch_listener.height = PAGE_ARROW_SIZE + 2*PAGE_ARROW_CLICK_PADDING; 

    for(uint8_t i = 0; i < APPS_PER_PAGE; i++) {
        int32_t row = (i / 2);
        int32_t col = i % 2;

        int32_t x = APPS_MARGIN_X + col * (APP_WIDTH + APPS_SPACING_X);
        int32_t y = APPS_MARGIN_Y + row * (APP_HEIGHT + APPS_SPACING_Y);

        app_touch_listeners[i].x = x;
        app_touch_listeners[i].y = y;
        app_touch_listeners[i].width = APP_WIDTH;
        app_touch_listeners[i].height = APP_HEIGHT;
    }

    graphics.fillScreen(TFT_WHITE);
    this->draw_page();
}

void App_Drawer::onResume() {
    graphics.fillScreen(TFT_WHITE);
    this->draw_page();
}

void App_Drawer::onTick() {
    QueueSetMemberHandle_t activated = xQueueSelectFromSet(queue_set, portMAX_DELAY);

    if (activated == redraw_signal) {
        xSemaphoreTake(redraw_signal, 0);
        this->draw_page();
    } else if (activated == app_open_queue) {
        App * app;
        xQueueReceive(app_open_queue, &app, 0);

        if (app) {
            app->open();
            this->minimize();
        }
    } else if (activated == pins.gpio0) {
        xSemaphoreTake(pins.gpio0, 0);

        if (!this->minimized) {
            return;
        }

        xSemaphoreGive(minimize_signal);
        vTaskDelay(250 / portTICK_PERIOD_MS);

        this->open();
    }
}

void App_Drawer::onTouchTick() {
    TouchData data = touch.waitData();

    for(uint8_t i = 0; i < APPS_PER_PAGE; i++) {
        if(app_touch_listeners[i].contains(viewport, data)) {
            uint8_t start = page * APPS_PER_PAGE;
            uint8_t target = start + i;

            if (apps_length <= target) {
                continue;
            }
            
            App * app = apps[target];
            xQueueOverwrite(app_open_queue, &app);
            break;
        }
    }


    if (previous_page_touch_listener.contains(viewport, data)) {
        
        if (page > 0) {
            page--;
            xSemaphoreGive(redraw_signal);
            vTaskDelay(250 / portTICK_PERIOD_MS);
        }

    } else if (next_page_touch_listener.contains(viewport, data)) {
        
        uint8_t pages = ceil(apps_length / (float) APPS_PER_PAGE);
        if (page < pages - 1) {
            page++;
            xSemaphoreGive(redraw_signal);
            vTaskDelay(250 / portTICK_PERIOD_MS);
        }


    }
}

void App_Drawer::onClose() {
}

/*

*/

void App_Drawer::draw_page() {
    graphics.beginBatch();
    
    uint8_t pages = ceil(apps_length / (float) APPS_PER_PAGE);

    draw_right_arrow(page < pages - 1 ? TFT_BLACK : TFT_WHITE);
    draw_left_arrow(page > 0 ? TFT_BLACK : TFT_WHITE);

    uint8_t start = page * APPS_PER_PAGE;

    for(uint8_t i = 0; i < APPS_PER_PAGE; i++) {
        int32_t row = (i / 2);
        int32_t col = i % 2;

        int32_t x = APPS_MARGIN_X + col * (APP_WIDTH + APPS_SPACING_X);
        int32_t y = APPS_MARGIN_Y + row * (APP_HEIGHT + APPS_SPACING_Y);

        graphics.fillRectangle(x - APPS_SPACING_X / 2, y + APP_HEIGHT + 6, APP_WIDTH + APPS_SPACING_X, 20, TFT_WHITE);
        if (start + i > apps_length - 1) {
            graphics.fillRectangle(x, y, APP_WIDTH, APP_HEIGHT, TFT_WHITE);
            continue;
        }

        App * app = apps[start +i];

        graphics.fillRoundedRectangle(x, y, APP_WIDTH, APP_HEIGHT, APP_BORDER_RADIUS, app->color);
        graphics.drawString(x + APP_WIDTH / 2, y + APP_HEIGHT + 6, app->name.c_str(), TFT_BLACK, 2, TC_DATUM);
    }

    graphics.endBatch();
}

void App_Drawer::draw_left_arrow(int32_t color) {
    graphics.fillTriangle(
        PAGE_ARROW_MARGIN_X, 
        VIEW_HEIGHT - PAGE_ARROW_MARGIN_Y,
        PAGE_ARROW_MARGIN_X + PAGE_ARROW_HEIGHT,
        VIEW_HEIGHT - PAGE_ARROW_MARGIN_Y - PAGE_ARROW_SIZE / 2,
        PAGE_ARROW_MARGIN_X + PAGE_ARROW_HEIGHT,
        VIEW_HEIGHT - PAGE_ARROW_MARGIN_Y + PAGE_ARROW_SIZE / 2,
        color
    );
}

void App_Drawer::draw_right_arrow(int32_t color) {
    graphics.fillTriangle(
        viewport.width - PAGE_ARROW_MARGIN_X, 
        VIEW_HEIGHT - PAGE_ARROW_MARGIN_Y,
        VIEW_WIDTH - PAGE_ARROW_MARGIN_X - PAGE_ARROW_HEIGHT,
        VIEW_HEIGHT - PAGE_ARROW_MARGIN_Y - PAGE_ARROW_SIZE / 2,
        VIEW_WIDTH - PAGE_ARROW_MARGIN_X - PAGE_ARROW_HEIGHT,
        VIEW_HEIGHT - PAGE_ARROW_MARGIN_Y + PAGE_ARROW_SIZE / 2,
        color
    );
}