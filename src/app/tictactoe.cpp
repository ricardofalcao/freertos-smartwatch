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


/*
    place array IDs:

    empty    : 0
    player 1 : 1
    player 2 : 2

    check_winner: 

    Player 1 wins : 1
    Player 2 wins : 2
    Draw          : 3  
    
*/

int places[9];

void _printWinningLine(int8_t pos) {
    if(pos < 4) graphics.drawLine(MARGIN_X, MARGIN_Y + CELL_HEIGHT/2 + CELL_HEIGHT*(pos-1), MARGIN_X + BOARD_WIDTH, MARGIN_Y + CELL_HEIGHT/2 + CELL_HEIGHT*(pos-1), TFT_BLACK);
    if(pos >= 4 && pos < 7) graphics.drawLine(MARGIN_X + CELL_WIDTH/2 + CELL_WIDTH*(pos-4), MARGIN_Y, MARGIN_X + CELL_WIDTH/2 + CELL_WIDTH*(pos-4), MARGIN_Y + BOARD_HEIGHT, TFT_BLACK);
    if(pos == 7)graphics.drawLine(MARGIN_X, MARGIN_Y, MARGIN_X + CELL_WIDTH, MARGIN_Y + CELL_HEIGHT, TFT_BLACK);
    if(pos == 8)graphics.drawLine(MARGIN_X, MARGIN_Y + CELL_HEIGHT, MARGIN_X + CELL_WIDTH, MARGIN_Y, TFT_BLACK);   
}

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

    graphics.drawLine(X, Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y, TFT_RED, 5);
    graphics.drawLine(X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y, TFT_RED, 5);


}

void _printO(int8_t cell) {

    int line = (int)floor(cell / 3.0f);
    int column = (cell % 3);

    int X = MARGIN_X + column*CELL_WIDTH + CELL_WIDTH/2;
    int Y = MARGIN_Y + line*CELL_HEIGHT + CELL_HEIGHT/2;

    graphics.drawCircle(X, Y, CELL_WIDTH/2 - MICRO_MARGIN_X, TFT_SKYBLUE, 5);

}

void Erase_Cell(int8_t cell) {
    int line = (int)floor(cell / 3.0f);
    int column = (cell % 3);

    int X = MARGIN_X + column*CELL_WIDTH + MICRO_MARGIN_X;
    int Y = MARGIN_Y + line*CELL_HEIGHT + MICRO_MARGIN_Y;

    graphics.fillRectangle(X, Y, X + CELL_WIDTH - 2 * MICRO_MARGIN_X, Y + CELL_HEIGHT - 2 * MICRO_MARGIN_Y, TFT_WHITE);
}

void placeX(int32_t cell) {
    places[cell] = 1;
}

void placeO(int32_t cell) {
    places[cell] = 2;
}

int checkWinner()
{
    bool draw_check = true;

    for(int i = 0; i < 9; i++) {
        if(places[i] != 0) {
            draw_check = false;
            break;
        }
    }

    if(draw_check) return 3;  
    
    if (places[0] != 0 && places[0] == places[1] && places[1] == places[2]) return places[0]*10 + 1; 

    if (places[3] != 0 && places[3] == places[4] && places[4] == places[5]) return places[3]*10 + 2;

    if (places[6] != 0 && places[6] == places[7] && places[6] == places[8]) return places[6]*10 + 3;

    if (places[0] != 0 && places[0] == places[3] && places[3] == places[6]) return places[1]*10 + 4;

    if (places[1] != 0 && places[1] == places[4] && places[1] == places[7]) return places[2]*10 + 5;

    if (places[2] != 0 && places[2] == places[5] && places[2] == places[8]) return places[0]*10 + 6;

    if (places[0] != 0 && places[0] == places[4] && places[0] == places[8]) return places[0]*10 + 7;

    if (places[2] != 0 && places[2] == places[4] && places[2] == places[6]) return places[2]*10 + 8;

    return 0;
    

}

App_TicTacToe::App_TicTacToe() : App("TicTacToe", "Let's play a game") {
    priority = 3;
    stack_depth = 2048;
}

void App_TicTacToe::onOpen() {
    Serial.println("[TicTacToe] OPEN");


}

void App_TicTacToe::onTick() {

    Serial.println("[TicTacToe] Tick");

    int play = 0;

    for(int i=0; i<9;i++) Erase_Cell(i);       
    
    while(true) {
        printf("Player 1 make your Move: ");
        scanf("%d", &play);

        placeX(play);
        _printX(play);

        if (checkWinner() != 0)
        {
            if (checkWinner() == 3) printf("It's a draw!");
            else {
                if(checkWinner() > 10 && checkWinner() < 19 ) { //player 1 wins
                    printf("Player 1 wins!");
                    _printWinningLine(checkWinner() - 10);
                }
                else{                                           //player 2 wins
                    printf("Player 1 wins!");
                    _printWinningLine(checkWinner() - 20);
                }
            break;
            }
        }
    
        printf("Player 2 make your Move: ");
        scanf("%d", &play);

        placeO(play);
        _printO(play);

        if (checkWinner() != 0)
        {
            if (checkWinner() == 3) printf("It's a draw!");
            else {
                if(checkWinner() > 10 && checkWinner() < 19 ) { //player 1 wins
                    printf("Player 1 wins!");
                    _printWinningLine(checkWinner() - 10);
                }
                else{                                           //player 2 wins
                    printf("Player 1 wins!");
                    _printWinningLine(checkWinner() - 20);
                }
            break;
            }
        }

    }



}

void App_TicTacToe::onClose() {
    Serial.println("[TicTacToe] Closed");
}


