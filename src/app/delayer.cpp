#include "app/delayer.h"

#include "tft.h"
#include "graphics.h"
#include "notifications.h"
#include "pins.h"

#define BOARD_WIDTH (DEFAULT_VIEWPORT.width)
#define BOARD_HEIGHT (DEFAULT_VIEWPORT.height)

#define MARGIN_X (int)(BOARD_WIDTH / 2.0)
#define MARGIN_Y (int)(BOARD_HEIGHT / 2.0)

#define TRIANGLE_HEIGHT 30
#define TRIANGLE_WIDTH 40
#define TRIANGLE_MARGIN 10
#define TRIANGLES_DISTANCE 50

#define NUMBERS_HEIGHT 20

#define DOT_THICKNESS 1

#define BOTTOM_Y (MARGIN_Y + NUMBERS_HEIGHT + TRIANGLE_HEIGHT + 50)

#define CIRCLE_RADIUS 30
#define PLAY_TRIANGLE_OFFSET 4

#define PAUSE_BARS_DISTANCE 20
#define PAUSE_BARS_WIDTH 5
#define PAUSE_BARS_HEIGHT 40

#define STOP_BUTTON_SIDE_LENGHT 40

#define MAX_HOURS 24
#define MAX_MINUTES 59
#define MAX_SECONDS 59
#define MIN_TIME 0

#define EVENT_RESUME_DELAYER 0x80
#define EVENT_PAUSE_DELAYER 0x40
#define EVENT_STOP_DELAYER 0x20

#define BUZZ_CYCLES         1
#define BUZZ_HZ             5
#define BUZZ_DURATION_MS    100

#define STATE_WAITING 0
#define STATE_PLAYING 1
#define STATE_STOP 2

App_Delayer::App_Delayer() : App(MSG_DELAYER_NAME, MSG_DELAYER_DESCRIPTION)
{
    priority = 4;
    stack_depth = 10240;
    touch_stack_depth = 10240;

    button_touch_listeners[0].x = MARGIN_X - TRIANGLES_DISTANCE - TRIANGLE_WIDTH / 2;
    button_touch_listeners[0].y = MARGIN_Y - TRIANGLE_HEIGHT;
    button_touch_listeners[0].width = TRIANGLE_WIDTH;
    button_touch_listeners[0].height = TRIANGLE_HEIGHT;

    button_touch_listeners[1].x = MARGIN_X - TRIANGLE_WIDTH / 2;
    button_touch_listeners[1].y = MARGIN_Y - TRIANGLE_HEIGHT;
    button_touch_listeners[1].width = TRIANGLE_WIDTH;
    button_touch_listeners[1].height = TRIANGLE_HEIGHT;

    button_touch_listeners[2].x = MARGIN_X + TRIANGLES_DISTANCE - TRIANGLE_WIDTH / 2;
    button_touch_listeners[2].y = MARGIN_Y - TRIANGLE_HEIGHT;
    button_touch_listeners[2].width = TRIANGLE_WIDTH;
    button_touch_listeners[2].height = TRIANGLE_HEIGHT;

    button_touch_listeners[3].x = MARGIN_X - TRIANGLE_WIDTH / 2 - TRIANGLES_DISTANCE;
    button_touch_listeners[3].y = MARGIN_Y + TRIANGLE_MARGIN + NUMBERS_HEIGHT;
    button_touch_listeners[3].width = TRIANGLE_WIDTH;
    button_touch_listeners[3].height = TRIANGLE_HEIGHT;

    button_touch_listeners[4].x = MARGIN_X - TRIANGLE_WIDTH / 2;
    button_touch_listeners[4].y = MARGIN_Y + TRIANGLE_MARGIN + NUMBERS_HEIGHT;
    button_touch_listeners[4].width = TRIANGLE_WIDTH;
    button_touch_listeners[4].height = TRIANGLE_HEIGHT;

    button_touch_listeners[5].x = MARGIN_X - TRIANGLE_WIDTH / 2 + TRIANGLES_DISTANCE;
    button_touch_listeners[5].y = MARGIN_Y + TRIANGLE_MARGIN + NUMBERS_HEIGHT;
    button_touch_listeners[5].width = TRIANGLE_WIDTH;
    button_touch_listeners[5].height = TRIANGLE_HEIGHT;

    button_touch_listeners[6].x = MARGIN_X / 2 - CIRCLE_RADIUS;
    button_touch_listeners[6].y = BOTTOM_Y + TRIANGLE_MARGIN;
    button_touch_listeners[6].width = 2 * CIRCLE_RADIUS;
    button_touch_listeners[6].height = 2 * CIRCLE_RADIUS;

    button_touch_listeners[7].x = MARGIN_X + MARGIN_X / 2 - CIRCLE_RADIUS;
    button_touch_listeners[7].y = BOTTOM_Y + TRIANGLE_MARGIN;
    button_touch_listeners[7].width = 2 * CIRCLE_RADIUS;
    button_touch_listeners[7].height = 2 * CIRCLE_RADIUS;
}

void App_Delayer::draw_arrows(GBatch_t *batch, uint32_t color)
{

    //hours top button
    batch->fillTriangle(MARGIN_X - TRIANGLES_DISTANCE, MARGIN_Y - TRIANGLE_HEIGHT, MARGIN_X - TRIANGLE_WIDTH / 2 - TRIANGLES_DISTANCE, MARGIN_Y - TRIANGLE_MARGIN, MARGIN_X + TRIANGLE_WIDTH / 2 - TRIANGLES_DISTANCE, MARGIN_Y - TRIANGLE_MARGIN, color);
    //minutes top button
    batch->fillTriangle(MARGIN_X, MARGIN_Y - TRIANGLE_HEIGHT, MARGIN_X - TRIANGLE_WIDTH / 2, MARGIN_Y - TRIANGLE_MARGIN, MARGIN_X + TRIANGLE_WIDTH / 2, MARGIN_Y - TRIANGLE_MARGIN, color);
    //seconds top button
    batch->fillTriangle(MARGIN_X + TRIANGLES_DISTANCE, MARGIN_Y - TRIANGLE_HEIGHT, MARGIN_X - TRIANGLE_WIDTH / 2 + TRIANGLES_DISTANCE, MARGIN_Y - TRIANGLE_MARGIN, MARGIN_X + TRIANGLE_WIDTH / 2 + TRIANGLES_DISTANCE, MARGIN_Y - TRIANGLE_MARGIN, color);

    //hours bottom button
    batch->fillTriangle(MARGIN_X - TRIANGLES_DISTANCE, MARGIN_Y + TRIANGLE_HEIGHT + NUMBERS_HEIGHT, MARGIN_X - TRIANGLE_WIDTH / 2 - TRIANGLES_DISTANCE, MARGIN_Y + TRIANGLE_MARGIN + NUMBERS_HEIGHT, MARGIN_X + TRIANGLE_WIDTH / 2 - TRIANGLES_DISTANCE, MARGIN_Y + TRIANGLE_MARGIN + NUMBERS_HEIGHT, color);
    //minutes bottom button
    batch->fillTriangle(MARGIN_X, MARGIN_Y + TRIANGLE_HEIGHT + NUMBERS_HEIGHT, MARGIN_X - TRIANGLE_WIDTH / 2, MARGIN_Y + TRIANGLE_MARGIN + NUMBERS_HEIGHT, MARGIN_X + TRIANGLE_WIDTH / 2, MARGIN_Y + TRIANGLE_MARGIN + NUMBERS_HEIGHT, color);
    //seconds bottom button
    batch->fillTriangle(MARGIN_X + TRIANGLES_DISTANCE, MARGIN_Y + TRIANGLE_HEIGHT + NUMBERS_HEIGHT, MARGIN_X - TRIANGLE_WIDTH / 2 + TRIANGLES_DISTANCE, MARGIN_Y + TRIANGLE_MARGIN + NUMBERS_HEIGHT, MARGIN_X + TRIANGLE_WIDTH / 2 + TRIANGLES_DISTANCE, MARGIN_Y + TRIANGLE_MARGIN + NUMBERS_HEIGHT, color);
}

void App_Delayer::draw_test(GBatch_t *batch)
{
    for (uint8_t i = 0; i < DELAYER_CELL_TOUCH_LISTENERS; i++)
    {
        RectangleTouchListener listener = button_touch_listeners[i];
        batch->drawRectangle(listener.x, listener.y, listener.width, listener.height, 2);
    }
}
void App_Delayer::draw_hours(GBatch_t *batch)
{

    char _hours[15];

    sprintf(_hours, " %02d ", hours);
    batch->drawFilledString(MARGIN_X - TRIANGLES_DISTANCE, MARGIN_Y + NUMBERS_HEIGHT / 2, _hours, TFT_BLACK, TFT_WHITE, 3, MC_DATUM);
}

void App_Delayer::draw_minutes(GBatch_t *batch)
{

    char _minutes[15];

    sprintf(_minutes, " %02d ", minutes);
    batch->drawFilledString(MARGIN_X, MARGIN_Y + NUMBERS_HEIGHT / 2, _minutes, TFT_BLACK, TFT_WHITE, 3, MC_DATUM);
}

void App_Delayer::draw_seconds(GBatch_t *batch)
{

    char _seconds[15];

    sprintf(_seconds, " %02d ", seconds);
    batch->drawFilledString(MARGIN_X + TRIANGLES_DISTANCE, MARGIN_Y + NUMBERS_HEIGHT / 2, _seconds, TFT_BLACK, TFT_WHITE, 3, MC_DATUM);
}

void App_Delayer::draw_play_button(GBatch_t *batch)
{
    batch->fillCircle(BOARD_WIDTH - MARGIN_X / 2, BOTTOM_Y + CIRCLE_RADIUS + TRIANGLE_MARGIN, CIRCLE_RADIUS, TFT_BLACK);
    batch->fillTriangle(BOARD_WIDTH - MARGIN_X / 2 - PLAY_TRIANGLE_OFFSET, BOTTOM_Y + PLAY_TRIANGLE_OFFSET + TRIANGLE_MARGIN,
                        BOARD_WIDTH - MARGIN_X / 2 - PLAY_TRIANGLE_OFFSET, BOTTOM_Y + TRIANGLE_MARGIN + 2 * CIRCLE_RADIUS - PLAY_TRIANGLE_OFFSET,
                        BOARD_WIDTH - MARGIN_X / 2 - PLAY_TRIANGLE_OFFSET + CIRCLE_RADIUS, BOTTOM_Y + TRIANGLE_MARGIN + CIRCLE_RADIUS, TFT_WHITE);
}

void App_Delayer::draw_pause_button(GBatch_t *batch)
{
    batch->fillCircle(BOARD_WIDTH - MARGIN_X / 2, BOTTOM_Y + CIRCLE_RADIUS + TRIANGLE_MARGIN, CIRCLE_RADIUS, TFT_BLACK);
    batch->fillRectangle(BOARD_WIDTH - MARGIN_X / 2 - PAUSE_BARS_DISTANCE / 2 - PAUSE_BARS_WIDTH, BOTTOM_Y + TRIANGLE_MARGIN + 2*PLAY_TRIANGLE_OFFSET, PAUSE_BARS_WIDTH, PAUSE_BARS_HEIGHT, TFT_WHITE);
    batch->fillRectangle(BOARD_WIDTH - MARGIN_X / 2 + PAUSE_BARS_DISTANCE / 2, BOTTOM_Y + TRIANGLE_MARGIN + 2*PLAY_TRIANGLE_OFFSET, PAUSE_BARS_WIDTH, PAUSE_BARS_HEIGHT, TFT_WHITE);
}

void App_Delayer::draw_stop_button(GBatch_t *batch)
{
    batch->fillCircle(MARGIN_X / 2, BOTTOM_Y + CIRCLE_RADIUS + TRIANGLE_MARGIN, CIRCLE_RADIUS, TFT_BLACK);
    batch->fillRectangle(MARGIN_X / 2 - STOP_BUTTON_SIDE_LENGHT / 2, BOTTOM_Y + CIRCLE_RADIUS + TRIANGLE_MARGIN - STOP_BUTTON_SIDE_LENGHT / 2, STOP_BUTTON_SIDE_LENGHT, STOP_BUTTON_SIDE_LENGHT, TFT_WHITE);
}

void App_Delayer::draw_doubledots(GBatch_t *batch)
{
    //between hours and minutes
    batch->fillCircle(MARGIN_X - TRIANGLES_DISTANCE / 2, MARGIN_Y + NUMBERS_HEIGHT / 3, DOT_THICKNESS, TFT_BLACK);
    batch->fillCircle(MARGIN_X - TRIANGLES_DISTANCE / 2, MARGIN_Y + 2 * NUMBERS_HEIGHT / 3, DOT_THICKNESS, TFT_BLACK);

    //between minutes and seconds
    batch->fillCircle(MARGIN_X + TRIANGLES_DISTANCE / 2, MARGIN_Y + NUMBERS_HEIGHT / 3, DOT_THICKNESS, TFT_BLACK);
    batch->fillCircle(MARGIN_X + TRIANGLES_DISTANCE / 2, MARGIN_Y + 2 * NUMBERS_HEIGHT / 3, DOT_THICKNESS, TFT_BLACK);
}

int App_Delayer::check_click_button(TouchData data)
{
    if (!data.pressed)
    {
        return -1;
    }

    for (int i = 0; i < DELAYER_CELL_TOUCH_LISTENERS; i++)
    {

        if (button_touch_listeners[i].contains(DEFAULT_VIEWPORT, data))
        {
            return i;
        }
    }

    return -1;
}

void App_Delayer::timeout_beep(note_t note, uint8_t octave)
{
    notifications.enqueueNotification(lang.get(MSG_DELAYER_TIMEOUT));

    if (xSemaphoreTake(pins.buzzer_mutex, portMAX_DELAY) == pdTRUE) {
        for(uint8_t i = 0;  i < BUZZ_CYCLES * BUZZ_HZ; i++) {
            ledcWriteNote(0, note, octave);
            
            if (vAppConditionalDelay(BUZZ_DURATION_MS / portTICK_RATE_MS, EVENT_STOP_DELAYER | EVENT_PAUSE_DELAYER)) {
                break;
            }

            ledcWrite(0, 0);

            if (vAppConditionalDelay((1000 / BUZZ_HZ - BUZZ_DURATION_MS) / portTICK_RATE_MS, EVENT_STOP_DELAYER | EVENT_PAUSE_DELAYER)) {
                break;
            }
        }

        vAppConditionalDelay(250 / portTICK_RATE_MS, EVENT_STOP_DELAYER | EVENT_PAUSE_DELAYER);

        xSemaphoreGive(pins.buzzer_mutex);
    }

}
void App_Delayer::onOpen()
{   
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    batch.fillScreen(TFT_WHITE);

    draw_arrows(&batch, TFT_BLACK);
    draw_hours(&batch);
    draw_minutes(&batch);
    draw_seconds(&batch);
    draw_doubledots(&batch);
    draw_play_button(&batch);
    draw_stop_button(&batch);

    //draw_test(&batch);

    graphics.endBatch(&batch);
}

void App_Delayer::onResume()
{
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    batch.fillScreen(TFT_WHITE);

    draw_hours(&batch);
    draw_minutes(&batch);
    draw_seconds(&batch);
    draw_doubledots(&batch);
    draw_stop_button(&batch);

    if (state == STATE_STOP)
        draw_arrows(&batch, TFT_BLACK);

    if (state == STATE_WAITING || state == STATE_STOP)
        draw_play_button(&batch);

    if (state == STATE_PLAYING)
        draw_pause_button(&batch);

    graphics.endBatch(&batch);
}

void App_Delayer::onTick()
{
    //PLAYING --> COUNTDOWN
    if (state == STATE_PLAYING)
    {
        if (hours == 0)
        {
            if ((minutes > 0 && seconds > 0) || (minutes == 0 && seconds > 0))
            {
                seconds = seconds - 1;

                if (!minimized) {
                    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                    draw_seconds(&batch);
                    graphics.endBatch(&batch);
                }
            }
            else if (minutes > 0 && seconds == 0)
            {
                seconds = 59;
                minutes = minutes - 1;

                if (!minimized) {
                    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                    draw_seconds(&batch);
                    draw_minutes(&batch);
                    graphics.endBatch(&batch);
                }
            }
            else if (minutes == 0 && seconds == 0)
            {
                timeout_beep(NOTE_A, 4);
                xEventGroupSetBits(event_group, EVENT_STOP_DELAYER);
            }
        }

        else if (hours > 0)
        {
            if ((minutes > 0 && seconds > 0) || (minutes == 0 && seconds > 0))
            {
                seconds = seconds - 1;

                if (!minimized) {
                    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                    draw_seconds(&batch);
                    graphics.endBatch(&batch);
                }
            }
            else if (minutes > 0 && seconds == 0)
            {
                seconds = 59;
                minutes = minutes - 1;

                if (!minimized) {
                    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                    draw_seconds(&batch);
                    draw_minutes(&batch);
                    graphics.endBatch(&batch);
                }
            }
            else if (minutes == 0 && seconds == 0)
            {
                minutes = 59;
                seconds = 59;
                hours = hours - 1;

                if (!minimized) {
                    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                    draw_seconds(&batch);
                    draw_minutes(&batch);
                    draw_hours(&batch);
                    graphics.endBatch(&batch);
                }
            }
        }
    }

    EventBits_t bits = vAppConditionalDelay(1000 / portTICK_RATE_MS, EVENT_STOP_DELAYER | EVENT_RESUME_DELAYER | EVENT_PAUSE_DELAYER);

    if (bits & EVENT_STOP_DELAYER) {
        xEventGroupClearBits(event_group, EVENT_STOP_DELAYER);

        hours = aux_h;
        minutes = aux_m;
        seconds = aux_s;
        state = STATE_STOP;

        if(!minimized) {
            GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
            draw_arrows(&batch, TFT_BLACK);
            draw_play_button(&batch);
            draw_seconds(&batch);
            draw_minutes(&batch);
            draw_hours(&batch);
            graphics.endBatch(&batch);
        }

    } else if (bits & EVENT_PAUSE_DELAYER) {
        xEventGroupClearBits(event_group, EVENT_PAUSE_DELAYER);

        state = STATE_WAITING;

        if (!minimized) {
            GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
            draw_arrows(&batch, TFT_WHITE);
            draw_play_button(&batch);
            graphics.endBatch(&batch);
        }

    } else if (bits & EVENT_RESUME_DELAYER) {
        xEventGroupClearBits(event_group, EVENT_RESUME_DELAYER);

        if (state == STATE_STOP) {
            aux_h = hours;
            aux_m = minutes;
            aux_s = seconds;
        }

        state = STATE_PLAYING;

        if (!minimized) {
            GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
            draw_arrows(&batch, TFT_WHITE);
            draw_pause_button(&batch);
            graphics.endBatch(&batch);
        }

    }
}

void App_Delayer::onTouchTick()
{
    TouchData data = touch.waitPress();
    unsigned long start = millis();

    int pressed_button = check_click_button(data);

    switch (pressed_button)
    {
    case 0:
        if (state != STATE_STOP)
        {
            return;
        }

        while (touch.get().pressed)
        {
            if (millis() - start > 1500)
            {
                hours = min(hours + 5, MAX_HOURS);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_hours(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                hours = min(hours + 1, MAX_HOURS);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_hours(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
        }
        break;

    case 1:
        if (state != STATE_STOP)
        {
            return;
        }

        while (touch.get().pressed)
        {
            if (millis() - start > 1500)
            {
                minutes = min(minutes + 5, MAX_MINUTES);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_minutes(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                minutes = min(minutes + 1, MAX_MINUTES);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_minutes(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
        }
        break;

    case 2:
        if (state != STATE_STOP)
        {
            return;
        }

        while (touch.get().pressed)
        {
            if (millis() - start > 1500)
            {
                seconds = min(seconds + 5, MAX_SECONDS);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_seconds(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                seconds = min(seconds + 1, MAX_SECONDS);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_seconds(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
        }
        break;

    case 3:
        if (state != STATE_STOP)
        {
            return;
        }

        while (touch.get().pressed)
        {
            if (millis() - start > 1500)
            {
                hours = max(hours - 5, MIN_TIME);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_hours(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                hours = max(hours - 1, MIN_TIME);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_hours(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
        }
        break;

    case 4:
        if (state != STATE_STOP)
        {
            return;
        }

        while (touch.get().pressed)
        {
            if (millis() - start > 1500)
            {
                minutes = max(minutes - 5, MIN_TIME);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_minutes(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                minutes = max(minutes - 1, MIN_TIME);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_minutes(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
        }
        break;

    case 5:
        if (state != STATE_STOP)
        {
            return;
        }

        while (touch.get().pressed)
        {
            if (millis() - start > 1500)
            {
                seconds = max(seconds - 5, MIN_TIME);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_seconds(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                seconds = max(seconds - 1, MIN_TIME);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                draw_seconds(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
        }
        break;

    case 6:
        touch.waitRelease();

        if (state == STATE_STOP)
        {
            return;
        }

        xEventGroupSetBits(event_group, EVENT_STOP_DELAYER);
        break;

    case 7:
        touch.waitRelease();

        if (state == STATE_PLAYING)
        {
            xEventGroupSetBits(event_group, EVENT_PAUSE_DELAYER);
            return;
        }

        xEventGroupSetBits(event_group, EVENT_RESUME_DELAYER);
        return;
    }
}

void App_Delayer::onClose()
{
}