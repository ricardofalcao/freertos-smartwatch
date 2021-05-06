#include "app/tictactoe.h"

#include "tft.h"
#include "graphics.h"
#include "lang/lang.h"

#define MARGIN_X        35
#define MARGIN_Y        60

#define MICRO_MARGIN_X  10
#define MICRO_MARGIN_Y  10

#define BOARD_WIDTH     (VIEW_WIDTH - (MARGIN_X * 2))
#define BOARD_HEIGHT    (BOARD_WIDTH)

#define CELL_WIDTH      (int) (BOARD_WIDTH / 3.0)
#define CELL_HEIGHT     (int) (BOARD_HEIGHT / 3.0)

#define PLACE_X         1
#define PLACE_O         2

#define CELL_TOUCH_LISTENERS 9
#define CELL_TOUCH_LISTENER

#define STATE_DRAW      3

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
    
int winPos[8][3] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},

    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},

    {0, 4, 8},
    {2, 4, 6}
};

int places[9];
RectangleTouchListener cell_touch_listeners[CELL_TOUCH_LISTENERS];

RectangleTouchListener reset_touch_listener;

/*
    Graphics
*/

void App_TicTacToe::print_winning_line(int8_t layout) {
    //Serial.printf("Layout: %d\n", layout);

    if(layout < 3) {
        graphics.drawLine(MARGIN_X, MARGIN_Y + CELL_HEIGHT*layout + CELL_HEIGHT/2, MARGIN_X + BOARD_WIDTH, MARGIN_Y + CELL_HEIGHT*layout + CELL_HEIGHT/2, TFT_ORANGE, 6);
        return;
    }

    if(layout < 6) {
        graphics.drawLine(MARGIN_X + CELL_WIDTH/2 + CELL_WIDTH*(layout-3), MARGIN_Y, MARGIN_X + CELL_WIDTH/2 + CELL_WIDTH*(layout-3), MARGIN_Y + BOARD_HEIGHT, TFT_ORANGE, 6);
        return;
    }

    if(layout == 6) {
        graphics.drawLine(MARGIN_X, MARGIN_Y, MARGIN_X + BOARD_WIDTH, MARGIN_Y + BOARD_HEIGHT, TFT_ORANGE, 6);
        return;
    }

    graphics.drawLine(MARGIN_X, MARGIN_Y + BOARD_HEIGHT, MARGIN_X + BOARD_WIDTH, MARGIN_Y, TFT_ORANGE, 6);   
}

void App_TicTacToe::clear_grid() {
    graphics.fillRectangle(MARGIN_X - 10, MARGIN_Y - 10, BOARD_WIDTH + 10, BOARD_HEIGHT + 10, TFT_WHITE);
}

void App_TicTacToe::print_grid() {
    graphics.beginBatch();
    graphics.drawLine(MARGIN_X, MARGIN_Y + CELL_HEIGHT, MARGIN_X + BOARD_WIDTH, MARGIN_Y + CELL_HEIGHT, TFT_BLACK, 5);
    graphics.drawLine(MARGIN_X, MARGIN_Y + 2 * CELL_HEIGHT, MARGIN_X + BOARD_WIDTH, MARGIN_Y + 2 * CELL_HEIGHT, TFT_BLACK, 5);
    graphics.drawLine(MARGIN_X + CELL_WIDTH, MARGIN_Y, MARGIN_X + CELL_WIDTH, MARGIN_Y + BOARD_HEIGHT, TFT_BLACK, 5);
    graphics.drawLine(MARGIN_X + 2 * CELL_WIDTH, MARGIN_Y, MARGIN_X + 2 * CELL_WIDTH, MARGIN_Y + BOARD_HEIGHT, TFT_BLACK, 5);
    graphics.endBatch();
}

void App_TicTacToe::print_x(int8_t cell) {

    int line = (int)floor(cell / 3.0f);
    int column = (cell % 3);

    int X = MARGIN_X + column*CELL_WIDTH + MICRO_MARGIN_X;
    int Y = MARGIN_Y + line*CELL_HEIGHT + MICRO_MARGIN_Y;

    graphics.drawLine(X, Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y, TFT_RED, 5);
    graphics.drawLine(X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y, TFT_RED, 5);


}

void App_TicTacToe::print_o(int8_t cell) {

    int line = (int)floor(cell / 3.0f);
    int column = (cell % 3);

    int X = MARGIN_X + column*CELL_WIDTH + CELL_WIDTH/2;
    int Y = MARGIN_Y + line*CELL_HEIGHT + CELL_HEIGHT/2;

    graphics.drawCircle(X, Y, CELL_WIDTH/2 - MICRO_MARGIN_X, TFT_SKYBLUE, 5);

}

void App_TicTacToe::erase_cell(int8_t cell) {
    int line = (int)floor(cell / 3.0f);
    int column = (cell % 3);

    int X = MARGIN_X + column*CELL_WIDTH + MICRO_MARGIN_X;
    int Y = MARGIN_Y + line*CELL_HEIGHT + MICRO_MARGIN_Y;

    graphics.fillRectangle(X, Y, CELL_WIDTH - 2 * MICRO_MARGIN_X, CELL_HEIGHT - 2 * MICRO_MARGIN_Y, TFT_WHITE);
}

void App_TicTacToe::show_message(const char * message) {
    graphics.beginBatch();
    graphics.fillRectangle(0, MARGIN_Y + BOARD_HEIGHT + 40, VIEW_WIDTH, 40, TFT_WHITE);
    graphics.drawString(VIEW_WIDTH / 2, MARGIN_Y + BOARD_HEIGHT + 40 + 20 , message, TFT_BLACK, 2, MC_DATUM);
    graphics.endBatch();
}
/*
    Data structures
*/

void _place_x(int32_t cell) {
    places[cell] = PLACE_X;
}

void _place_o(int32_t cell) {
    places[cell] = PLACE_O;
}

/*
    Listeners
*/

int App_TicTacToe::check_click_cells(TouchData data) {
    for(int i = 0; i < 9; i++) {
        if (places[i] != 0) {
            continue;
        }

        if (cell_touch_listeners[i].contains(viewport, data)) {
            return i;
        }
    }

    return -1;
}

/*

*/

int _get_winner(uint8_t * layout)
{
    bool draw_check = true;

    for(int i = 0; i < 9; i++) {
        if(places[i] != 0) {
            draw_check = false;
            break;
        }
    }

    if(draw_check) {
        return 3;
    }  

    for(int i = 0; i < 8; i++) {
        int player = places[winPos[i][0]];
        if (player == 0) {
            continue;
        }

        bool valid = true;
        for(int j = 1; j < 3; j++) {
            if (places[winPos[i][j]] != player) {
                valid = false;
                break;
            }
        }

        if (valid) {
            *layout = i;
            return player;
        }
    }

    return 0;
}

/*

*/

App_TicTacToe::App_TicTacToe() : App("TicTacToe", "Let's play a game") {
    priority = 3;
    stack_depth = 4096;
}

void App_TicTacToe::onOpen() {
    graphics.fillScreen(TFT_WHITE);

    for(uint8_t i = 0; i < 9; i++) {
        int line = (int)floor(i / 3.0f);
        int column = (i % 3);

        int X = MARGIN_X + column*CELL_WIDTH;
        int Y = MARGIN_Y + line*CELL_HEIGHT;

        cell_touch_listeners[i].x = X;
        cell_touch_listeners[i].y = Y;
        cell_touch_listeners[i].width = CELL_WIDTH;
        cell_touch_listeners[i].height = CELL_HEIGHT;
    }

    print_grid();

    for(int i = 0; i < 9;i++) {
        erase_cell(i);
    }

    graphics.drawString(VIEW_WIDTH / 2, MARGIN_Y / 2, "Tic Tac Toe", TFT_BLACK, 3, MC_DATUM);
    graphics.fillRectangle(10, VIEW_HEIGHT - 40 - 10, VIEW_WIDTH - 2*10, 40, TFT_PURPLE);
    graphics.drawString(VIEW_WIDTH / 2, VIEW_HEIGHT - 20 - 10 - 2, "RESET", TFT_WHITE, 3, MC_DATUM);

    reset_touch_listener.x = 10;
    reset_touch_listener.y = VIEW_HEIGHT - 40 - 10;
    reset_touch_listener.width = VIEW_WIDTH - 2*10;
    reset_touch_listener.height = 40;

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
void App_TicTacToe::onTick() {
    if (ended) {
        TouchData data = touch.waitData();
        check_click_reset(data);
        return;
    }

    int play;

    show_message("Player 1 your turn!");
    //_show_message(lang.get(MSG_HELLO));

    do {
        TouchData data = touch.waitData();

        if (check_click_reset(data)) {
            return;
        }

        play = check_click_cells(data);
    } while(play == -1);

    _place_x(play);
    empty = false;
    print_x(play);

    if (check_winner()) {
        return;
    }

    show_message("Player 2 your turn!");

    do {
        TouchData data = touch.waitData();

        if (check_click_reset(data)) {
            return;
        }

        play = check_click_cells(data);
    } while(play == -1);

    _place_o(play);
    empty = false;
    print_o(play);

    if (check_winner()) {
        return;
    }
}

void App_TicTacToe::onClose() {
}

/*

*/

bool App_TicTacToe::check_winner() {
    uint8_t layout;
    int win = _get_winner(&layout);

    switch(win) {
        case STATE_DRAW: {
            //Serial.println("It's a draw!");
            show_message("It's a draw");
            return true;
        }

        case 2: {
            ended = true;
            //Serial.println("Player 2 wins!");
            print_winning_line(layout);
            show_message("Player 2 wins!");
            return true;
        }
        
        case 1: {
            ended = true;
            //Serial.println("Player 1 wins!");
            print_winning_line(layout);
            show_message("Player 1 wins!");
            return true;
        }
    }

    return false;
}

bool App_TicTacToe::check_click_reset(TouchData data) {
    if (empty) {
        return false;
    }

    if (reset_touch_listener.contains(viewport, data)) {
        for(int i = 0; i < 9; i++) {
            places[i] = 0;
        }

        empty = true;
        ended = false;

        clear_grid();
        print_grid();
        return true;
    }

    return false;
}

