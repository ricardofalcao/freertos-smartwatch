#include "app/clock.h"

#include "wifi.h"
#include "time.h"

#include "touch.h"
#include "graphics.h"

#define MARGIN_X    (DEFAULT_VIEWPORT.width / 2)
#define MARGIN_Y    (DEFAULT_VIEWPORT.height / 2 - 50)

#define BUTTON_WIDTH        (DEFAULT_VIEWPORT.width - 100)
#define BUTTON_HEIGHT       40

#define BUTTON_MARGIN_X     (DEFAULT_VIEWPORT.width / 2 - BUTTON_WIDTH / 2)
#define BUTTON_MARGIN_Y     (DEFAULT_VIEWPORT.height - 50 - BUTTON_HEIGHT)

#define ANALOG_FRAME_INNER_RADIUS   100
#define ANALOG_FRAME_OUTER_RADIUS   110

#define PI_1_6 0.5235987755982988f

#define EVENT_REFRESH_TYPE     0x80

App_Clock::App_Clock() : App(MSG_CLOCK_NAME, MSG_CLOCK_DESCRIPTION) {
    priority = 3;
    stack_depth = 10240;

    type_touch_listener.x = BUTTON_MARGIN_X;
    type_touch_listener.y = BUTTON_MARGIN_Y;
    type_touch_listener.width = BUTTON_WIDTH;
    type_touch_listener.height = BUTTON_HEIGHT;
}

void App_Clock::onOpen() {
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    batch.fillScreen(TFT_WHITE);
    draw_analog_frame(&batch);
    draw_type_button(&batch);

    graphics.endBatch(&batch);
}

void App_Clock::onResume() {
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    force_refresh = true;

    batch.fillScreen(TFT_WHITE);
    draw_analog_frame(&batch);
    draw_type_button(&batch);

    graphics.endBatch(&batch);
}

void App_Clock::draw_type_button(GBatch_t * batch) {

    switch (type) {
        case CLOCK_ANALOG: {
            batch->fillRoundedRectangle(BUTTON_MARGIN_X, BUTTON_MARGIN_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 5, TFT_ORANGE);
            batch->drawString(BUTTON_MARGIN_X + BUTTON_WIDTH / 2, BUTTON_MARGIN_Y + BUTTON_HEIGHT / 2 - 2, lang.get(MSG_CLOCK_TYPE_ANALOG) , TFT_BLACK, 3, MC_DATUM);
            break;
        }

        case CLOCK_DIGITAL: {
            batch->fillRoundedRectangle(BUTTON_MARGIN_X, BUTTON_MARGIN_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 5, TFT_GREEN);
            batch->drawString(BUTTON_MARGIN_X + BUTTON_WIDTH / 2, BUTTON_MARGIN_Y + BUTTON_HEIGHT / 2 - 2, lang.get(MSG_CLOCK_TYPE_DIGITAL) , TFT_BLACK, 3, MC_DATUM);
            break;
        }
    }
}

void App_Clock::draw_analog_frame(GBatch_t * batch) {
    batch->fillRectangle(MARGIN_X - ANALOG_FRAME_OUTER_RADIUS, MARGIN_Y - ANALOG_FRAME_OUTER_RADIUS, ANALOG_FRAME_OUTER_RADIUS * ANALOG_FRAME_OUTER_RADIUS, ANALOG_FRAME_OUTER_RADIUS * 2, TFT_WHITE);

    for(uint8_t i = 0; i < 12; i++) {
        float angle = PI_1_6 * i;

        int32_t x1 = ANALOG_FRAME_INNER_RADIUS * cos(angle);
        int32_t y1 = ANALOG_FRAME_INNER_RADIUS * sin(angle);
        int32_t x2 = ANALOG_FRAME_OUTER_RADIUS * cos(angle);
        int32_t y2 = ANALOG_FRAME_OUTER_RADIUS * sin(angle);

        batch->drawLine(MARGIN_X + x1, MARGIN_Y + y1, MARGIN_X + x2, MARGIN_Y + y2, TFT_BLACK, 3);
    }

    batch->fillCircle(MARGIN_X, MARGIN_Y, 3, TFT_BLACK);
}

void App_Clock::draw_digital_frame(GBatch_t * batch) {
    batch->fillRectangle(MARGIN_X - ANALOG_FRAME_OUTER_RADIUS, MARGIN_Y - ANALOG_FRAME_OUTER_RADIUS, ANALOG_FRAME_OUTER_RADIUS * ANALOG_FRAME_OUTER_RADIUS, ANALOG_FRAME_OUTER_RADIUS * 2, TFT_WHITE);
}

void App_Clock::draw_line(GBatch_t * batch, float angle, float size, uint32_t color) {

    int32_t x = size * cos(angle);
    int32_t y = size * sin(angle);

    batch->drawLine(MARGIN_X, MARGIN_Y, MARGIN_X + x, MARGIN_Y + y, color, 2);
}

void App_Clock::onTick() {
    
    if (!minimized) {
        switch (type) {
            case CLOCK_ANALOG: {
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

                old_time = current_time;

                time_t now;
                time(&now);
                localtime_r(&now, &current_time);
                // Serial.println(&current_time, "%A, %B %d %Y %H:%M:%S");

                if (force_refresh || old_time.tm_sec != current_time.tm_sec) {
                    float angle_s = old_time.tm_sec * 0.1047 - 1.5708;
                    draw_line(&batch, angle_s, 96, TFT_WHITE);

                    angle_s = current_time.tm_sec * 0.1047 - 1.5708;
                    draw_line(&batch, angle_s, 96, TFT_BLACK);
                }

                if (force_refresh || old_time.tm_min != current_time.tm_min) {
                    float angle_m = old_time.tm_min * 0.1047 - 1.5708;
                    draw_line(&batch, angle_m, 70, TFT_WHITE);
                    angle_m = current_time.tm_min * 0.1047 - 1.5708;
                    draw_line(&batch, angle_m, 70, TFT_BLACK);
                }

                if (force_refresh || old_time.tm_hour != current_time.tm_hour) {
                    float angle_h = ((old_time.tm_hour % 12) + old_time.tm_min / 60.0) * 0.5236 - 1.5708;
                    draw_line(&batch, angle_h, 50, TFT_WHITE);
                    angle_h = ((current_time.tm_hour % 12) + current_time.tm_min / 60.0) * 0.5236 - 1.5708;
                    draw_line(&batch, angle_h, 50, TFT_BLACK);
                }

                force_refresh = false;

                batch.fillCircle(MARGIN_X, MARGIN_Y, 3, TFT_BLACK);
                graphics.endBatch(&batch);
                break;
            }

            case CLOCK_DIGITAL: {
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

                old_time = current_time;

                time_t now;
                time(&now);
                localtime_r(&now, &current_time);
                
                char buffer[16];
                sprintf(buffer, "  %02d:%02d:%02d  ", current_time.tm_hour, current_time.tm_min, current_time.tm_sec);

                batch.drawFilledString(MARGIN_X, MARGIN_Y, buffer, TFT_BLACK, TFT_WHITE, 4, MC_DATUM);

                force_refresh = false;

                graphics.endBatch(&batch);
                break;
            }
        }
    }

    EventBits_t bits = vAppConditionalDelay(1000 / portTICK_RATE_MS, EVENT_REFRESH_TYPE);
    if (bits & EVENT_REFRESH_TYPE) {
        xEventGroupClearBits(event_group, EVENT_REFRESH_TYPE);

        GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
        force_refresh = true;

        switch (type) {
            case CLOCK_ANALOG: {
                draw_analog_frame(&batch);
                draw_type_button(&batch);
                break;
            }

            case CLOCK_DIGITAL: {
                draw_digital_frame(&batch);
                draw_type_button(&batch);
                break;
            }
        }

        graphics.endBatch(&batch);
    }
}

void App_Clock::onTouchTick() {
    TouchData data = touch.waitRelease();

    if (type_touch_listener.contains(DEFAULT_VIEWPORT, data)) {

        switch (type) {
            case CLOCK_ANALOG: {
                type = CLOCK_DIGITAL;
                break;
            }

            case CLOCK_DIGITAL: {
                type = CLOCK_ANALOG;
                break;
            }
        }

        xEventGroupSetBits(event_group, EVENT_REFRESH_TYPE);

    }
}

void App_Clock::onClose() {
}