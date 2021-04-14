#include "app/tictactoe.h"

#include "graphics.h"

#define MARGIN_X 35
#define MARGIN_Y 55

#define MICRO_MARGIN_X 10
#define MICRO_MARGIN_Y 10

#define BOARD_WIDTH TFT_WIDTH - (MARGIN_X * 2)
#define BOARD_HEIGHT BOARD_WIDTH

#define CELL_WIDTH BOARD_WIDTH/3
#define CELL_HEIGHT BOARD_HEIGHT/3

void _printGrid() {
    graphics.drawLine(MARGIN_X, MARGIN_Y + BOARD_HEIGHT/3, MARGIN_X + BOARD_WIDTH, MARGIN_Y + BOARD_HEIGHT/3, TFT_BLACK);
    graphics.drawLine(MARGIN_X, MARGIN_Y + BOARD_HEIGHT*2/3, MARGIN_X + BOARD_WIDTH, MARGIN_Y + BOARD_HEIGHT*2/3, TFT_BLACK);
    graphics.drawLine(MARGIN_X + BOARD_WIDTH/3, MARGIN_Y, MARGIN_X + BOARD_WIDTH/3, MARGIN_Y + BOARD_HEIGHT, TFT_BLACK);
    graphics.drawLine(MARGIN_X + BOARD_WIDTH*2/3, MARGIN_Y, MARGIN_X + BOARD_WIDTH*2/3, MARGIN_Y + BOARD_HEIGHT, TFT_BLACK);
}

void _printX(int8_t cell) {

    int line = (int)floor((cell - 1) / 3.0f) * 2;
    int column = (cell - 1) % 3 * 2;

    int X = MARGIN_X + column*CELL_WIDTH + MICRO_MARGIN_X;
    int Y = MARGIN_Y + line*CELL_HEIGHT + MICRO_MARGIN_Y;

    graphics.drawLine(X, Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y, TFT_RED);
    graphics.drawLine(X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y, TFT_RED);

}

void _printO(int8_t cell) {

    int line = (int)floor((cell - 1) / 3.0f) * 2;
    int column = (cell - 1) % 3 * 2;

    int X = MARGIN_X + column*CELL_WIDTH + CELL_WIDTH/2;
    int Y = MARGIN_Y + line*CELL_HEIGHT + CELL_HEIGHT/2;

    graphics.drawCircle(X,Y, CELL_WIDTH/2 - MICRO_MARGIN_X, TFT_SKYBLUE);

}

App_TicTacToe::App_TicTacToe() : App("TicTacToe", "Let's play a game") {
    priority = 3;
    stack_depth = 1024;
}

void App_TicTacToe::onOpen() {
    Serial.println("[Clock] OPEN");

    _printGrid();
    _printX(1);
    _printX(3);
    _printX(8);
    _printO(4);
    _printO(2);
}

void App_TicTacToe::onTick() {
    Serial.println("[TicTacToe] Tick");


}

void App_TicTacToe::onClose() {
    Serial.println("[TicTacToe] Closed");
}



