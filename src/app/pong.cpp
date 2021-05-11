#include "app/pong.h"

#include "graphics.h"
#include "touch.h"
#include "lang/lang.h"

#define BACKGROUND_COLOR        TFT_BLACK
#define GAME_COLOR              TFT_WHITE

#define BOARD_WIDTH             VIEW_WIDTH
#define BOARD_HEIGHT            VIEW_HEIGHT

#define MARGIN_Y                35
#define MARGIN_X                35

#define BAR_LENGTH              50
#define BAR_HALF_LENGTH         (BAR_LENGTH / 2)

#define BAR_HEIGHT              10
#define BAR_HALF_HEIGHT         (BAR_HEIGHT / 2)

#define SPACE_BETWEEN_BARS      6
#define DASHED_BARS_LENGTH      10

#define BALL_RADIUS             9
 

int bot_score = 0;
int player_score = 0;
/*
    Graphics
*/

void App_Pong::print_top_bar(int previous_x, int new_x) {
    if (new_x == previous_x) {
        return;
    }

    if (new_x > previous_x) {
        graphics.fillRectangle(previous_x - BAR_HALF_LENGTH, MARGIN_Y, new_x - previous_x, BAR_HEIGHT, BACKGROUND_COLOR);
    } else {
        graphics.fillRectangle(new_x + BAR_HALF_LENGTH, MARGIN_Y, previous_x - new_x, BAR_HEIGHT, BACKGROUND_COLOR);
    }

    //place_x means the center of the bar 
    graphics.fillRectangle(new_x - BAR_HALF_LENGTH, MARGIN_Y, BAR_LENGTH, BAR_HEIGHT, GAME_COLOR);
}

void App_Pong::print_bottom_bar(int previous_x, int new_x) {
    if (new_x == previous_x) {
        return;
    }

    if (new_x > previous_x) {
        graphics.fillRectangle(previous_x - BAR_HALF_LENGTH, BOARD_HEIGHT - MARGIN_Y - BAR_HEIGHT, new_x - previous_x, BAR_HEIGHT, BACKGROUND_COLOR);
    } else {
        graphics.fillRectangle(new_x + BAR_HALF_LENGTH, BOARD_HEIGHT - MARGIN_Y - BAR_HEIGHT, previous_x - new_x, BAR_HEIGHT, BACKGROUND_COLOR);
    }
    //place_x means the center of the bar 
    graphics.fillRectangle(new_x - BAR_HALF_LENGTH, BOARD_HEIGHT - MARGIN_Y - BAR_HEIGHT, BAR_LENGTH, BAR_HEIGHT, GAME_COLOR);
}

void App_Pong::print_field_lines(uint32_t color) {
    int numberOfLines = (BOARD_WIDTH - DASHED_BARS_LENGTH) / (DASHED_BARS_LENGTH + SPACE_BETWEEN_BARS) + 1;
    int offsetX = ((BOARD_WIDTH - DASHED_BARS_LENGTH) % (DASHED_BARS_LENGTH + SPACE_BETWEEN_BARS)) / 2;

    for(int i = 0; i < numberOfLines; i++) {
        int x = offsetX + i * (DASHED_BARS_LENGTH+SPACE_BETWEEN_BARS);

        graphics.drawLine(x, BOARD_HEIGHT / 2, x + DASHED_BARS_LENGTH, BOARD_HEIGHT / 2, color, 2);
    }
    
    graphics.drawLine(1, 1, 1, BOARD_HEIGHT, color, 8);
    graphics.drawLine(BOARD_WIDTH, 1, BOARD_WIDTH, BOARD_HEIGHT, color, 8);

}

void App_Pong::print_score(uint32_t color) {

    graphics.beginBatch();
    sprintf(_bot_score, "%d", bot_score);
    graphics.drawString(BOARD_WIDTH - MARGIN_X, BOARD_HEIGHT/2 - MARGIN_Y, _bot_score, color, 4, MC_DATUM);
    
    sprintf(_player_score, "%d", player_score);
    graphics.drawString(BOARD_WIDTH - MARGIN_X, BOARD_HEIGHT/2 + MARGIN_Y, _player_score, color, 4, MC_DATUM);
    graphics.endBatch();

}

void App_Pong::print_ball(int x_center, int y_center, uint32_t color) {
    graphics.fillCircle(x_center, y_center, BALL_RADIUS, color);
}

/*
    main
*/

App_Pong::App_Pong() : App("Pong", "Let's Play") {
    priority = 3;
    stack_depth = 4096;
    canMinimize = false;
}

 
void App_Pong::onOpen() {
    Serial.println("[Pong] OPEN");

    graphics.fillScreen(BACKGROUND_COLOR);
    print_field_lines(GAME_COLOR);

    print_score(GAME_COLOR);
}

float lerp(float a, float b, float x)
{ 
  return a + x * (b - a);
}

void App_Pong::onTick() {
    tick++;

    int nbotx = ((int) (sin(0.05 * tick) * (BOARD_WIDTH / 2 - BAR_LENGTH))) + BOARD_WIDTH / 2;

    graphics.beginBatch();
    if (nbotx != botx) {
        print_top_bar(botx, nbotx);
        botx = nbotx;
    }
  
    TouchData data = touch.get();

    if (data.pressed) {
        int nplayerx = constrain(data.x, BAR_HALF_LENGTH + 5, BOARD_WIDTH - BAR_HALF_LENGTH - 5);

        if (playerx != nplayerx) {
            tplayerx = nplayerx;
        }
    }

    int iiplayerx = ceil(lerp(playerx, tplayerx, 0.3));
    print_bottom_bar(playerx, iiplayerx);
    playerx = iiplayerx;

    graphics.endBatch();

    vAppDelay(10 / portTICK_PERIOD_MS);
}

void App_Pong::onClose() {
    Serial.println("[Pong] Closed");
}