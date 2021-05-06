#include "app/metronome.h"
#include "tft.h"
#include "graphics.h"

#define DURATION_MS 200
#define OUTPUT_PIN 22

#define BOARD_WIDTH     (VIEW_WIDTH)
#define BOARD_HEIGHT    (VIEW_HEIGHT)

#define MARGIN_X        (int) (BOARD_WIDTH / 2.0)
#define MARGIN_Y        50

#define TRIANGLE_SIDE           60
#define TRIANGLE_LOW_HEIGHT     (int) (TRIANGLE_SIDE * 0.2886751345948129f)
#define TRIANGLE_HIGH_HEIGHT    (int) (TRIANGLE_SIDE * 0.5773502691896257f)

#define INNER_TRIANGLE_SIDE     30
#define INNER_TRIANGLE_LOW_HEIGHT     (int) (INNER_TRIANGLE_SIDE * 0.2886751345948129f)
#define INNER_TRIANGLE_HIGH_HEIGHT    (int) (INNER_TRIANGLE_SIDE * 0.5773502691896257f)


#define MAX_BPM 300
#define MIN_BPM 10



App_Metronome::App_Metronome() : App("Metronome", "Time Marker") {
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
}
 
int bpm = 100;
int compass_type = 3;
void App_Metronome::print_button_up() {
    //triangle chape
    graphics.fillTriangle(MARGIN_X, MARGIN_Y - TRIANGLE_HIGH_HEIGHT, MARGIN_X - TRIANGLE_SIDE / 2, MARGIN_Y + TRIANGLE_LOW_HEIGHT, MARGIN_X + TRIANGLE_SIDE / 2, MARGIN_Y +  TRIANGLE_LOW_HEIGHT, TFT_BLACK);
    //inner triangle
    graphics.fillTriangle(MARGIN_X, MARGIN_Y - INNER_TRIANGLE_HIGH_HEIGHT, MARGIN_X - INNER_TRIANGLE_SIDE / 2, MARGIN_Y + INNER_TRIANGLE_LOW_HEIGHT, MARGIN_X + INNER_TRIANGLE_SIDE / 2, MARGIN_Y +  INNER_TRIANGLE_LOW_HEIGHT, TFT_WHITE);

}

void App_Metronome::print_button_down() {

    graphics.fillTriangle(MARGIN_X, BOARD_HEIGHT - MARGIN_Y + TRIANGLE_HIGH_HEIGHT, MARGIN_X - TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - TRIANGLE_LOW_HEIGHT, MARGIN_X + TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - TRIANGLE_LOW_HEIGHT, TFT_BLACK);
    //inner triangle
    graphics.fillTriangle(MARGIN_X, BOARD_HEIGHT - MARGIN_Y + INNER_TRIANGLE_HIGH_HEIGHT, MARGIN_X - INNER_TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - INNER_TRIANGLE_LOW_HEIGHT, MARGIN_X + INNER_TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - INNER_TRIANGLE_LOW_HEIGHT, TFT_WHITE);
}

void App_Metronome::print_bpm() {
    graphics.beginBatch();
    graphics.drawString(VIEW_WIDTH / 2, VIEW_HEIGHT / 2, bpm_print_buffer, TFT_WHITE, 4, MC_DATUM);
    
    sprintf(bpm_print_buffer, "%d BPM", bpm);
    graphics.drawString(VIEW_WIDTH / 2, VIEW_HEIGHT / 2, bpm_print_buffer, TFT_BLACK, 4, MC_DATUM);
    graphics.endBatch();
}

int App_Metronome::check_click_button(TouchData data) {
    if (!data.pressed) {
        return -1;
    }

    for(int i = 0; i < CELL_TOUCH_LISTENERS; i++) {
        
        if (button_touch_listeners[i].contains(viewport, data)) {
            return i;
        }
    }

    return -1;
}

void beep_output(note_t note, uint8_t octave) {
    
    ledcWriteNote(0, note, octave);
    vTaskDelay(DURATION_MS / portTICK_PERIOD_MS);
    ledcWrite(0, 0);
}

void App_Metronome::onOpen() {
    ledcAttachPin(OUTPUT_PIN, 0);
    
    graphics.fillScreen(TFT_WHITE);
    print_button_up();

    print_bpm();

    print_button_down();
}

void App_Metronome::onTick() {
    beep_output(NOTE_A, 5);

    for(int i=0;i<compass_type;i++) {
        vTaskDelay((60*1000/bpm - DURATION_MS) / portTICK_PERIOD_MS);
        beep_output(NOTE_E, 5);
    } 
    
    vTaskDelay((60*1000/bpm - DURATION_MS) / portTICK_PERIOD_MS);
}

void App_Metronome::onTouchTick() {
    TouchData data = touch.waitData();
    unsigned long start = millis();

    int pressed_button = check_click_button(data);
    Serial.printf("Pressed: %d\n", pressed_button);

    switch (pressed_button)
    {
    case 0:
        while (touch.getData().pressed)
        {
            if (millis() - start > 2000)
            {
                bpm = min(bpm + 5, MAX_BPM);
                print_bpm();

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                bpm = min(bpm + 1, MAX_BPM);
                print_bpm();

                vTaskDelay(200 / portTICK_PERIOD_MS);
            }
        }
        break;

    case 1:
        while (touch.getData().pressed)
        {
            if (millis() - start > 2000)
            {
                bpm = max(bpm - 5, MIN_BPM);
                print_bpm();

                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                bpm = max(bpm - 1, MIN_BPM);
                print_bpm();

                vTaskDelay(200 / portTICK_PERIOD_MS);
            }
        }
        break;

        default: break;
    }
}

void App_Metronome::onClose() {
    ledcDetachPin(OUTPUT_PIN);
    
}