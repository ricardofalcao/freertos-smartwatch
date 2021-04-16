#include "app/metronome.h"
#include "graphics.h"

#define DURATION_MS 200
#define OUTPUT_PIN 13

#define BOARD_WIDTH     (TFT_WIDTH)
#define BOARD_HEIGHT    (TFT_HEIGHT)

#define MARGIN_X        (int) (BOARD_WIDTH / 2.0)
#define MARGIN_Y        50

#define TRINGLE_HEIGHT  40
#define TRINGLE_WIDTH   60

#define INNER_TRIANGLE_MARGIN  25

App_Metronome::App_Metronome() : App("Metronome", "Time Marker") {
    priority = 3;
    stack_depth = 1024;
}
 
int bpm = 100;
int compass_type = 3;
void _print_button_up() {
    //triangle chape
    graphics.fillTriangle(MARGIN_X, MARGIN_Y, MARGIN_X - TRINGLE_WIDTH/2, MARGIN_Y + TRINGLE_HEIGHT, MARGIN_X + TRINGLE_WIDTH/2, MARGIN_Y + TRINGLE_HEIGHT, TFT_BLACK);
    //inner triangle
    graphics.fillTriangle(MARGIN_X, MARGIN_Y + INNER_TRIANGLE_MARGIN, MARGIN_X - TRINGLE_WIDTH/2 + INNER_TRIANGLE_MARGIN, MARGIN_Y + TRINGLE_HEIGHT - INNER_TRIANGLE_MARGIN, MARGIN_X + TRINGLE_WIDTH/2 - INNER_TRIANGLE_MARGIN, MARGIN_Y + TRINGLE_HEIGHT - INNER_TRIANGLE_MARGIN, TFT_WHITE);
}


void _print_button_down() {

    graphics.fillTriangle(MARGIN_X, BOARD_HEIGHT - MARGIN_Y, MARGIN_X - TRINGLE_WIDTH/2, BOARD_HEIGHT - MARGIN_Y - TRINGLE_HEIGHT, MARGIN_X + TRINGLE_WIDTH/2, BOARD_HEIGHT-  MARGIN_Y - TRINGLE_HEIGHT, TFT_BLACK);
    //inner triangle
    graphics.fillTriangle(MARGIN_X, BOARD_HEIGHT - MARGIN_Y - INNER_TRIANGLE_MARGIN, MARGIN_X - TRINGLE_WIDTH/2 - INNER_TRIANGLE_MARGIN, BOARD_HEIGHT - MARGIN_Y - TRINGLE_HEIGHT + INNER_TRIANGLE_MARGIN, MARGIN_X + TRINGLE_WIDTH/2 - INNER_TRIANGLE_MARGIN, BOARD_HEIGHT - MARGIN_Y - TRINGLE_HEIGHT + INNER_TRIANGLE_MARGIN, TFT_WHITE);}

void beep_output(note_t note, uint8_t octave) {
    
    ledcWriteNote(0, note, octave);
    vTaskDelay(DURATION_MS / portTICK_PERIOD_MS);
    ledcWrite(0, 0);
}

void App_Metronome::onOpen() {
    Serial.println("[Metronome] OPEN");
    ledcAttachPin(OUTPUT_PIN, 0);

}


void App_Metronome::onTick() {

    beep_output(NOTE_A, 5);

    for(int i=0;i<compass_type;i++) {
        vTaskDelay((bpm/60*1000 - DURATION_MS) / portTICK_PERIOD_MS);
        beep_output(NOTE_E, 5);
    } 
    vTaskDelay((bpm/60*1000 - DURATION_MS) / portTICK_PERIOD_MS);
}

void App_Metronome::onClose() {
    Serial.println("[Metronome] Closed");
    ledcDetachPin(OUTPUT_PIN);
    
}