#include "app/metronome.h"
#include "graphics.h"

#define DURATION_MS 200
#define OUTPUT_PIN 13

#define BOARD_WIDTH     (TFT_WIDTH)
#define BOARD_HEIGHT    (TFT_HEIGHT)

#define MARGIN_X        (int) (BOARD_WIDTH / 2.0)
#define MARGIN_Y        50

#define TRIANGLE_SIDE           60
#define TRIANGLE_LOW_HEIGHT     (int) (TRIANGLE_SIDE * 0.2886751345948129f)
#define TRIANGLE_HIGH_HEIGHT    (int) (TRIANGLE_SIDE * 0.5773502691896257f)

#define INNER_TRIANGLE_SIDE     30
#define INNER_TRIANGLE_LOW_HEIGHT     (int) (INNER_TRIANGLE_SIDE * 0.2886751345948129f)
#define INNER_TRIANGLE_HIGH_HEIGHT    (int) (INNER_TRIANGLE_SIDE * 0.5773502691896257f)

#define CELL_TOUCH_LISTENERS 2

#define MAX_BPM 300
#define MIN_BPM 10


RectangleTouchListener button_touch_listeners[CELL_TOUCH_LISTENERS];

App_Metronome::App_Metronome() : App("Metronome", "Time Marker") {
    priority = 3;
    stack_depth = 4096;
}
 
int bpm = 100;
int compass_type = 3;
void App_Metronome::print_button_up() {
    //triangle chape
    graphics.fillTriangle(viewport, MARGIN_X, MARGIN_Y - TRIANGLE_HIGH_HEIGHT, MARGIN_X - TRIANGLE_SIDE / 2, MARGIN_Y + TRIANGLE_LOW_HEIGHT, MARGIN_X + TRIANGLE_SIDE / 2, MARGIN_Y +  TRIANGLE_LOW_HEIGHT, TFT_BLACK);
    //inner triangle
    graphics.fillTriangle(viewport, MARGIN_X, MARGIN_Y - INNER_TRIANGLE_HIGH_HEIGHT, MARGIN_X - INNER_TRIANGLE_SIDE / 2, MARGIN_Y + INNER_TRIANGLE_LOW_HEIGHT, MARGIN_X + INNER_TRIANGLE_SIDE / 2, MARGIN_Y +  INNER_TRIANGLE_LOW_HEIGHT, TFT_WHITE);

}

void App_Metronome::print_button_down() {

    graphics.fillTriangle(viewport, MARGIN_X, BOARD_HEIGHT - MARGIN_Y + TRIANGLE_HIGH_HEIGHT, MARGIN_X - TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - TRIANGLE_LOW_HEIGHT, MARGIN_X + TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - TRIANGLE_LOW_HEIGHT, TFT_BLACK);
    //inner triangle
    graphics.fillTriangle(viewport, MARGIN_X, BOARD_HEIGHT - MARGIN_Y + INNER_TRIANGLE_HIGH_HEIGHT, MARGIN_X - INNER_TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - INNER_TRIANGLE_LOW_HEIGHT, MARGIN_X + INNER_TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - INNER_TRIANGLE_LOW_HEIGHT, TFT_WHITE);
}

int _check_click_button(TouchData data) {

    for(int i = 0; i < CELL_TOUCH_LISTENERS; i++) {
        
        if (button_touch_listeners[i].contains(data)) {
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
    
    graphics.fillScreen(viewport, TFT_WHITE);
    print_button_up();

    char buffer[16];
    sprintf(buffer, "%d BPM", bpm);

    graphics.drawString(viewport, TFT_WIDTH / 2, TFT_HEIGHT / 2, buffer, TFT_BLACK, 4, MC_DATUM);
    print_button_down();
}

void App_Metronome::onTick() {
    beep_output(NOTE_A, 5);

    for(uint8_t i = 0; i < CELL_TOUCH_LISTENERS; i++) {
        button_touch_listeners[i].x = MARGIN_X - TRIANGLE_SIDE/2;
        button_touch_listeners[i].y = MARGIN_Y - TRIANGLE_HIGH_HEIGHT + i*(BOARD_HEIGHT - 3*MARGIN_Y);
        button_touch_listeners[i].width = TRIANGLE_SIDE;
        button_touch_listeners[i].height = TRIANGLE_HIGH_HEIGHT + TRIANGLE_LOW_HEIGHT;
    }

    for(int i=0;i<compass_type;i++) {
        vTaskDelay((60*1000/bpm - DURATION_MS) / portTICK_PERIOD_MS);
        beep_output(NOTE_E, 5);
    } 
    
    vTaskDelay((60*1000/bpm - DURATION_MS) / portTICK_PERIOD_MS);
}

void App_Metronome::onTouchTick() {
    TouchData data = touch.waitData();
    unsigned long start = millis();

    int pressed_button = _check_click_button(data);

    switch (pressed_button)
    {
    case 0:
        while (touch.getData().pressed)
        {
            if (millis() - start > 2000)
            {
                bpm = min(bpm + 5, MAX_BPM);
                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                bpm = min(bpm + 1, MAX_BPM);
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
                vTaskDelay(400 / portTICK_PERIOD_MS);
            }
            else
            {
                bpm = max(bpm - 1, MIN_BPM);
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