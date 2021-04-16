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

App_Metronome::App_Metronome() : App("Metronome", "Time Marker") {
    priority = 3;
    stack_depth = 4096;
}
 
int bpm = 100;
int compass_type = 3;
void _print_button_up() {
    //triangle chape
    graphics.fillTriangle(MARGIN_X, MARGIN_Y - TRIANGLE_HIGH_HEIGHT, MARGIN_X - TRIANGLE_SIDE / 2, MARGIN_Y + TRIANGLE_LOW_HEIGHT, MARGIN_X + TRIANGLE_SIDE / 2, MARGIN_Y +  TRIANGLE_LOW_HEIGHT, TFT_BLACK);
    //inner triangle
    graphics.fillTriangle(MARGIN_X, MARGIN_Y - INNER_TRIANGLE_HIGH_HEIGHT, MARGIN_X - INNER_TRIANGLE_SIDE / 2, MARGIN_Y + INNER_TRIANGLE_LOW_HEIGHT, MARGIN_X + INNER_TRIANGLE_SIDE / 2, MARGIN_Y +  INNER_TRIANGLE_LOW_HEIGHT, TFT_WHITE);

}

void _print_button_down() {

    graphics.fillTriangle(MARGIN_X, BOARD_HEIGHT - MARGIN_Y + TRIANGLE_HIGH_HEIGHT, MARGIN_X - TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - TRIANGLE_LOW_HEIGHT, MARGIN_X + TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - TRIANGLE_LOW_HEIGHT, TFT_BLACK);
    //inner triangle
    graphics.fillTriangle(MARGIN_X, BOARD_HEIGHT - MARGIN_Y + INNER_TRIANGLE_HIGH_HEIGHT, MARGIN_X - INNER_TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - INNER_TRIANGLE_LOW_HEIGHT, MARGIN_X + INNER_TRIANGLE_SIDE / 2, BOARD_HEIGHT - MARGIN_Y - INNER_TRIANGLE_LOW_HEIGHT, TFT_WHITE);
}

void beep_output(note_t note, uint8_t octave) {
    
    ledcWriteNote(0, note, octave);
    vTaskDelay(DURATION_MS / portTICK_PERIOD_MS);
    ledcWrite(0, 0);
}

void App_Metronome::onOpen() {
    ledcAttachPin(OUTPUT_PIN, 0);
    
    graphics.fillScreen(TFT_WHITE);
    _print_button_up();

    char buffer[16];
    sprintf(buffer, "%d BPM", bpm);

    graphics.drawString(TFT_WIDTH / 2, TFT_HEIGHT / 2, "TESTE", TFT_BLACK, 4, MC_DATUM);
    _print_button_down();
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

}

void App_Metronome::onClose() {
    ledcDetachPin(OUTPUT_PIN);
    
}