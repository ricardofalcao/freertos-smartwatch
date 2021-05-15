#include "app/metronome.h"
#include "tft.h"
#include "graphics.h"

#define DURATION_MS 200
#define OUTPUT_PIN 22

#define BOARD_WIDTH     (DEFAULT_VIEWPORT.width)
#define BOARD_HEIGHT    (DEFAULT_VIEWPORT.height)

#define MARGIN_X        (int) (BOARD_WIDTH / 2.0)
#define MARGIN_Y        50

#define TRIANGLE_SIDE           60
#define TRIANGLE_LOW_HEIGHT     (int) (TRIANGLE_SIDE * 0.2886751345948129f)
#define TRIANGLE_HIGH_HEIGHT    (int) (TRIANGLE_SIDE * 0.5773502691896257f)

#define INNER_TRIANGLE_SIDE     30
#define INNER_TRIANGLE_LOW_HEIGHT     (int) (INNER_TRIANGLE_SIDE * 0.2886751345948129f)
#define INNER_TRIANGLE_HIGH_HEIGHT    (int) (INNER_TRIANGLE_SIDE * 0.5773502691896257f)

#define CIRCLE_RADIUS           30
#define PLAY_TRAINGLE_OFFSET    10

#define PAUSE_BARS_OFFSET       10
#define PAUSE_BARS_DISTANCE     10
#define PAUSE_BARS_WIDTH        20
#define PAUSE_BARS_HEIGHT       40

#define MAX_BPM 200
#define MIN_BPM 30

/*
    button_on = 0 -> pause mode : draw play button
    button_on = 1 -> play mode : draw pause button

    System starts on pause mode
*/

App_Metronome::App_Metronome() : App(MSG_METRONOMER_NAME, MSG_METRONOMER_DESCRIPTION) {
    priority = 3;
    stack_depth = 10240;
    touch_stack_depth = 10240;

    button_touch_listeners[0].x = MARGIN_X - TRIANGLE_SIDE/2;
    button_touch_listeners[0].y = MARGIN_Y - TRIANGLE_HIGH_HEIGHT;
    button_touch_listeners[0].width = TRIANGLE_SIDE;
    button_touch_listeners[0].height = TRIANGLE_SIDE;

    button_touch_listeners[1].x = MARGIN_X - TRIANGLE_SIDE/2;
    button_touch_listeners[1].y = BOARD_HEIGHT - MARGIN_Y - TRIANGLE_LOW_HEIGHT;
    button_touch_listeners[1].width = TRIANGLE_SIDE;
    button_touch_listeners[1].height = TRIANGLE_SIDE;

    button_touch_listeners[2].x = BOARD_WIDTH - CIRCLE_RADIUS;
    button_touch_listeners[2].y = BOARD_HEIGHT - CIRCLE_RADIUS;
    button_touch_listeners[2].width = CIRCLE_RADIUS*2;
    button_touch_listeners[2].height = CIRCLE_RADIUS*2;

    button_touch_listeners[3].x = 0;
    button_touch_listeners[3].y = BOARD_HEIGHT - CIRCLE_RADIUS;
    button_touch_listeners[3].width = CIRCLE_RADIUS*2;
    button_touch_listeners[3].height = CIRCLE_RADIUS*2;

    resume_event = xSemaphoreCreateBinary();
}
 


void App_Metronome::print_button_up(GBatch_t * batch) {
    //triangle chape
    batch->fillTriangle(MARGIN_X, MARGIN_Y - TRIANGLE_HIGH_HEIGHT, MARGIN_X - TRIANGLE_SIDE / 2, MARGIN_Y + TRIANGLE_LOW_HEIGHT, MARGIN_X + TRIANGLE_SIDE / 2, MARGIN_Y +  TRIANGLE_LOW_HEIGHT, TFT_BLACK);
    //inner triangle
    batch->fillTriangle(MARGIN_X, MARGIN_Y - INNER_TRIANGLE_HIGH_HEIGHT, MARGIN_X - INNER_TRIANGLE_SIDE / 2, MARGIN_Y + INNER_TRIANGLE_LOW_HEIGHT, MARGIN_X + INNER_TRIANGLE_SIDE / 2, MARGIN_Y +  INNER_TRIANGLE_LOW_HEIGHT, TFT_WHITE);

}

void App_Metronome::print_button_down(GBatch_t * batch) {

    batch->fillTriangle(MARGIN_X, BOARD_HEIGHT - MARGIN_Y + TRIANGLE_HIGH_HEIGHT, MARGIN_X - TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - TRIANGLE_LOW_HEIGHT, MARGIN_X + TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - TRIANGLE_LOW_HEIGHT, TFT_BLACK);
    //inner triangle
    batch->fillTriangle(MARGIN_X, BOARD_HEIGHT - MARGIN_Y + INNER_TRIANGLE_HIGH_HEIGHT, MARGIN_X - INNER_TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - INNER_TRIANGLE_LOW_HEIGHT, MARGIN_X + INNER_TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - INNER_TRIANGLE_LOW_HEIGHT, TFT_WHITE);
}

void App_Metronome::print_bpm(GBatch_t * batch) {
    batch->drawString(DEFAULT_VIEWPORT.width / 2, DEFAULT_VIEWPORT.height / 2, bpm_print_buffer, TFT_WHITE, 4, MC_DATUM);
    
    sprintf(bpm_print_buffer, "%d BPM", bpm);
    batch->drawString(DEFAULT_VIEWPORT.width / 2, DEFAULT_VIEWPORT.height / 2, bpm_print_buffer, TFT_BLACK, 4, MC_DATUM);
}

void App_Metronome::print_play_button(GBatch_t * batch) {
    batch->fillCircle(BOARD_WIDTH - CIRCLE_RADIUS, BOARD_HEIGHT - CIRCLE_RADIUS, CIRCLE_RADIUS, TFT_BLACK);
    batch->fillTriangle(BOARD_WIDTH - CIRCLE_RADIUS + PLAY_TRAINGLE_OFFSET, BOARD_HEIGHT - 2*CIRCLE_RADIUS + PLAY_TRAINGLE_OFFSET,
                        BOARD_WIDTH - CIRCLE_RADIUS + PLAY_TRAINGLE_OFFSET, BOARD_HEIGHT - PLAY_TRAINGLE_OFFSET, 
                        BOARD_WIDTH - CIRCLE_RADIUS + 2*PLAY_TRAINGLE_OFFSET, BOARD_HEIGHT - CIRCLE_RADIUS, TFT_WHITE);
}

void App_Metronome::print_pause_button(GBatch_t * batch) {
    batch->fillCircle(BOARD_WIDTH - CIRCLE_RADIUS, BOARD_HEIGHT - CIRCLE_RADIUS, CIRCLE_RADIUS, TFT_BLACK);
    batch->fillRectangle(BOARD_WIDTH - CIRCLE_RADIUS + PAUSE_BARS_OFFSET, BOARD_HEIGHT - CIRCLE_RADIUS + PAUSE_BARS_OFFSET, PAUSE_BARS_WIDTH, PAUSE_BARS_HEIGHT, TFT_WHITE);
    batch->fillRectangle(BOARD_WIDTH - CIRCLE_RADIUS + PAUSE_BARS_OFFSET + PAUSE_BARS_WIDTH + PAUSE_BARS_DISTANCE, BOARD_HEIGHT - CIRCLE_RADIUS + PAUSE_BARS_OFFSET, PAUSE_BARS_WIDTH, PAUSE_BARS_HEIGHT, TFT_WHITE);
}

void App_Metronome::print_time4_string(GBatch_t * batch) {

    batch->fillCircle(CIRCLE_RADIUS, BOARD_HEIGHT - CIRCLE_RADIUS, CIRCLE_RADIUS, TFT_BLACK);
    sprintf(time_signature, "%d", compass_type);
    batch->drawString(CIRCLE_RADIUS, BOARD_HEIGHT - CIRCLE_RADIUS, time_signature, TFT_WHITE, 4, TL_DATUM);
}
void App_Metronome::print_time3_string(GBatch_t * batch) {
    batch->fillCircle(CIRCLE_RADIUS, BOARD_HEIGHT - CIRCLE_RADIUS, CIRCLE_RADIUS, TFT_BLACK);
    sprintf(time_signature, "%d", compass_type);
    batch->drawString(CIRCLE_RADIUS, BOARD_HEIGHT - CIRCLE_RADIUS, time_signature, TFT_WHITE, 4, TL_DATUM);
}

int App_Metronome::check_click_button(TouchData data) {
    if (!data.pressed) {
        return -1;
    }

    for(int i = 0; i < METRONOME_CELL_TOUCH_LISTENERS; i++) {
        
        if (button_touch_listeners[i].contains(DEFAULT_VIEWPORT, data)) {
            return i;
        }
    }

    return -1;
}

void App_Metronome::beep_output(note_t note, uint8_t octave) {
    
    ledcWriteNote(0, note, octave);
    vTaskDelay(DURATION_MS / portTICK_PERIOD_MS);
    ledcWrite(0, 0);
}

void App_Metronome::onOpen() {
    ledcAttachPin(OUTPUT_PIN, 0);

    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
    batch.fillScreen(TFT_WHITE);

    print_button_up(&batch);
    print_bpm(&batch);
    print_button_down(&batch);
    print_play_button(&batch);

    graphics.endBatch(&batch);
}

void App_Metronome::onResume() {
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
    batch.fillScreen(TFT_WHITE);

    print_button_up(&batch);
    print_bpm(&batch);
    print_button_down(&batch);

    if(button_on == 0)  print_play_button(&batch);
    if(button_on == 1)  print_pause_button(&batch);

    graphics.endBatch(&batch);
}

void App_Metronome::onTick() {

    if(button_on == 1) {
        beep_output(NOTE_A, 5);

        for(int i=0;i<compass_type;i++) {
            vAppDelay((60*1000/bpm - DURATION_MS) / portTICK_PERIOD_MS);
            if(button_on == 0) {
                return;
            }
            beep_output(NOTE_E, 5);
        } 
    
        vAppDelay((60*1000/bpm - DURATION_MS) / portTICK_PERIOD_MS);

        return;
    }
    
    xSemaphoreTake(resume_event, portMAX_DELAY);
    
   
}

void App_Metronome::onTouchTick() {
    TouchData data = touch.waitPress();
    unsigned long start = millis();

    int pressed_button = check_click_button(data);

    switch (pressed_button)
    {
    case 0:
        while (touch.get().pressed)
        {
            if (millis() - start > 1500)
            {
                bpm = min(bpm + 5, MAX_BPM);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                print_bpm(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                bpm = min(bpm + 1, MAX_BPM);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                print_bpm(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(200 / portTICK_PERIOD_MS);
            }
        }
        break;

    case 1:
        while (touch.get().pressed)
        {
            if (millis() - start > 1500)
            {
                bpm = max(bpm - 5, MIN_BPM);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                print_bpm(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                bpm = max(bpm - 1, MIN_BPM);
                GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
                print_bpm(&batch);
                graphics.endBatch(&batch);

                vTaskDelay(200 / portTICK_PERIOD_MS);
            }
        }
        break;

    case 2: 
        if(button_on == 0) {
            button_on = 1;

            GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
            print_pause_button(&batch);
            graphics.endBatch(&batch);
        }

        else {
            button_on = 0;
            GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
            print_play_button(&batch);
            graphics.endBatch(&batch);

            xSemaphoreGive(resume_event);
        }
        break;
    case 3:
        if(compass_type == 3) {
            compass_type = 4;
            GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
            print_time4_string(&batch);
            graphics.endBatch(&batch);
        }
        else {
            compass_type = 3;
            GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
            print_time3_string(&batch);
            graphics.endBatch(&batch);
        }
        default: break;
    }
}

void App_Metronome::onClose() {
    ledcDetachPin(OUTPUT_PIN);
    
}