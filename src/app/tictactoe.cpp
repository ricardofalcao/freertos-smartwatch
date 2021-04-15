#include "app/tictactoe.h"

#include "graphics.h"

#define MARGIN_X 35
#define MARGIN_Y 35

#define MICRO_MARGIN_X 10
#define MICRO_MARGIN_Y 10

#define BOARD_WIDTH     (TFT_WIDTH - (MARGIN_X * 2))
#define BOARD_HEIGHT    (BOARD_WIDTH)

#define CELL_WIDTH      (int) (BOARD_WIDTH / 3.0)
#define CELL_HEIGHT     (int) (BOARD_HEIGHT / 3.0)

void _printGrid() {
    graphics.drawLine(MARGIN_X, MARGIN_Y + CELL_HEIGHT, MARGIN_X + BOARD_WIDTH, MARGIN_Y + CELL_HEIGHT, TFT_BLACK, 5);
    graphics.drawLine(MARGIN_X, MARGIN_Y + 2 * CELL_HEIGHT, MARGIN_X + BOARD_WIDTH, MARGIN_Y + 2 * CELL_HEIGHT, TFT_BLACK, 5);
    graphics.drawLine(MARGIN_X + CELL_WIDTH, MARGIN_Y, MARGIN_X + CELL_WIDTH, MARGIN_Y + BOARD_HEIGHT, TFT_BLACK, 5);
    graphics.drawLine(MARGIN_X + 2 * CELL_WIDTH, MARGIN_Y, MARGIN_X + 2 * CELL_WIDTH, MARGIN_Y + BOARD_HEIGHT, TFT_BLACK, 5);
}

void _printX(int8_t cell) {

    int line = (int)floor(cell / 3.0f);
    int column = (cell % 3);

    int X = MARGIN_X + column*CELL_WIDTH + MICRO_MARGIN_X;
    int Y = MARGIN_Y + line*CELL_HEIGHT + MICRO_MARGIN_Y;

    Serial.printf("%d %d %d\n", MARGIN_X, MICRO_MARGIN_X, CELL_WIDTH);
    Serial.printf("Line: %d, Col: %d, X: %d, Y: %d, X2: %d, Y2: %d\n", line, column, X, Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y);

    graphics.drawLine(X, Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y, TFT_RED, 5);
    graphics.drawLine(X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y, TFT_RED, 5);

}

void _printO(int8_t cell) {

    int line = (int)floor(cell / 3.0f);
    int column = (cell % 3);

    int X = MARGIN_X + column*CELL_WIDTH + CELL_WIDTH/2;
    int Y = MARGIN_Y + line*CELL_HEIGHT + CELL_HEIGHT/2;

    graphics.drawCircle(X,Y, CELL_WIDTH/2 - MICRO_MARGIN_X, TFT_SKYBLUE, 5);

}

App_TicTacToe::App_TicTacToe() : App("TicTacToe", "Let's play a game") {
    priority = 3;
    stack_depth = 2048;
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


}

void App_TicTacToe::onClose() {
    Serial.println("[TicTacToe] Closed");
}



