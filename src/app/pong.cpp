#include "app/pong.h"

#include "graphics.h"
#include "lang/lang.h"


#define CELL_WIDTH tft.width()
#define CELL_HEIGHT tft.height()

#define MARGIN_Y 35
#define MIDDLE_CELL_WIDTH (int) (CELL_WIDTH / 2.0)

#define BAR_HALF_LENGHT 25
#define SPACE_BTW_BARS 6
#define DASHED_BARS_LENGHT 10

#define BALL_RADIUS 9


/*
    Graphics
*/

void App_Pong::print_top_bar(int place_x) {
    //place_x means the center of the bar 
    graphics.drawLine(place_x - BAR_HALF_LENGHT, MARGIN_Y, place_x + BAR_HALF_LENGHT, MARGIN_Y + CELL_HEIGHT/2, TFT_BLACK, 10);
}

void App_Pong::print_bottom_bar(int place_x) {
    //place_x means the center of the bar 
    graphics.drawLine(place_x - BAR_HALF_LENGHT,CELL_HEIGHT - MARGIN_Y, place_x + BAR_HALF_LENGHT, CELL_HEIGHT - MARGIN_Y + CELL_HEIGHT/2, TFT_BLACK, 10);
}

void App_Pong::print_dashed() {
    int lenght = 0;

    graphics.beginBatch();
    for(int i=0;i*(DASHED_BARS_LENGHT+SPACE_BTW_BARS) + DASHED_BARS_LENGHT<CELL_WIDTH;i++) {
        graphics.drawLine(i*(DASHED_BARS_LENGHT+SPACE_BTW_BARS) , MIDDLE_CELL_WIDTH, i*(DASHED_BARS_LENGHT+SPACE_BTW_BARS) + DASHED_BARS_LENGHT, MIDDLE_CELL_WIDTH, TFT_BLACK, 2);
    }
    graphics.endBatch();
}

void App_Pong::print_ball(int x_center, int y_center) {
    graphics.fillCircle(x_center, y_center, BALL_RADIUS, TFT_CYAN);
}

void App_Pong::erase_top_bar(int place_x) {
    //place_x means the center of the bar 
    graphics.drawLine(place_x - BAR_HALF_LENGHT, MARGIN_Y, place_x + BAR_HALF_LENGHT, MARGIN_Y + CELL_HEIGHT/2, TFT_WHITE, 10);
}

void App_Pong::erase_bottom_bar(int place_x) {
    //place_x means the center of the bar 
    graphics.drawLine(place_x - BAR_HALF_LENGHT,CELL_HEIGHT - MARGIN_Y, place_x + BAR_HALF_LENGHT, CELL_HEIGHT - MARGIN_Y + CELL_HEIGHT/2, TFT_WHITE, 10);
}

void App_Pong::erase_ball(int x_center, int y_center) {
    graphics.fillCircle(x_center, y_center, BALL_RADIUS, TFT_WHITE);
}

App_Pong::App_Pong() : App("Pong", "Let's Play") {
    priority = 3;
    stack_depth = 1024;
}

 
void App_Pong::onOpen() {
    Serial.println("[Pong] OPEN");

    print_dashed();
}

void App_Pong::onTick() {


}

void App_Pong::onClose() {
    Serial.println("[Pong] Closed");
}