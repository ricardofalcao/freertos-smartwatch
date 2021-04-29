#include "app/pong.h"

#include "graphics.h"
#include "lang/lang.h"

#define BACKGROUND_COLOR        TFT_BLACK
#define GAME_COLOR              TFT_WHITE

#define BOARD_WIDTH             tft.width()
#define BOARD_HEIGHT            tft.height()

#define MARGIN_Y                35

#define BAR_LENGTH              50
#define BAR_HALF_LENGTH         (BAR_LENGTH / 2)

#define SPACE_BETWEEN_BARS      6
#define DASHED_BARS_LENGTH      10

#define BALL_RADIUS             9


/*
    Graphics
*/

void App_Pong::print_top_bar(int place_x, uint32_t color) {
    //place_x means the center of the bar 
    graphics.drawLine(place_x - BAR_HALF_LENGTH, MARGIN_Y, place_x + BAR_HALF_LENGTH, MARGIN_Y, color, 10);
}

void App_Pong::print_bottom_bar(int place_x, uint32_t color) {
    //place_x means the center of the bar 
    graphics.drawLine(place_x - BAR_HALF_LENGTH, BOARD_HEIGHT - MARGIN_Y, place_x + BAR_HALF_LENGTH, BOARD_HEIGHT - MARGIN_Y, color, 10);
}

void App_Pong::print_dashed(uint32_t color) {
    graphics.beginBatch();

    int numberOfLines = (BOARD_WIDTH - DASHED_BARS_LENGTH) / (DASHED_BARS_LENGTH + SPACE_BETWEEN_BARS);
    for(int i = 0; i < numberOfLines; i++) {
        int x = i * (DASHED_BARS_LENGTH+SPACE_BETWEEN_BARS);

        graphics.drawLine(x, BOARD_HEIGHT / 2, x + DASHED_BARS_LENGTH, BOARD_HEIGHT / 2, color, 2);
    }

    graphics.endBatch();
}

void App_Pong::print_ball(int x_center, int y_center, uint32_t color) {
    graphics.fillCircle(x_center, y_center, BALL_RADIUS, color);
}

App_Pong::App_Pong() : App("Pong", "Let's Play") {
    priority = 3;
    stack_depth = 1024;
}

 
void App_Pong::onOpen() {
    Serial.println("[Pong] OPEN");

    graphics.fillScreen(BACKGROUND_COLOR);
    print_dashed(GAME_COLOR);
}

void App_Pong::onTick() {
    tick++;

    int nbotx = ((int) sin(3.1416e-03 * tick) * (BOARD_WIDTH / 2 - BAR_LENGTH)) + BOARD_WIDTH / 2;

    if (nbotx == botx) {
        return;
    }

    graphics.beginBatch();
    print_top_bar(botx, BACKGROUND_COLOR);
    botx = nbotx;
    print_top_bar(botx, GAME_COLOR);
    graphics.endBatch();
}

void App_Pong::onClose() {
    Serial.println("[Pong] Closed");
}