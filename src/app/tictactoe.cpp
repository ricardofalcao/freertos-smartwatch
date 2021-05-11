#include "app/tictactoe.h"

#include "tft.h"
#include "graphics.h"
#include "lang/lang.h"

#define MARGIN_X        35
#define MARGIN_Y        60

#define MICRO_MARGIN_X  10
#define MICRO_MARGIN_Y  10

#define BOARD_WIDTH     (DEFAULT_VIEWPORT.width - (MARGIN_X * 2))
#define BOARD_HEIGHT    (BOARD_WIDTH)

#define CELL_WIDTH      (int) (BOARD_WIDTH / 3.0)
#define CELL_HEIGHT     (int) (BOARD_HEIGHT / 3.0)

#define PLACE_X         1
#define PLACE_O         2

#define STATE_DRAW      3

#define EVENT_DRAW_CELL 0x80

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

/*
    Graphics
*/

void App_TicTacToe::print_winning_line(int8_t layout) {
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    if(layout < 3) {
        batch.drawLine(MARGIN_X, MARGIN_Y + CELL_HEIGHT*layout + CELL_HEIGHT/2, MARGIN_X + BOARD_WIDTH, MARGIN_Y + CELL_HEIGHT*layout + CELL_HEIGHT/2, TFT_ORANGE, 6);
        graphics.endBatch(&batch);
        return;
    }

    if(layout < 6) {
        batch.drawLine(MARGIN_X + CELL_WIDTH/2 + CELL_WIDTH*(layout-3), MARGIN_Y, MARGIN_X + CELL_WIDTH/2 + CELL_WIDTH*(layout-3), MARGIN_Y + BOARD_HEIGHT, TFT_ORANGE, 6);
        graphics.endBatch(&batch);
        return;
    }

    if(layout == 6) {
        batch.drawLine(MARGIN_X, MARGIN_Y, MARGIN_X + BOARD_WIDTH, MARGIN_Y + BOARD_HEIGHT, TFT_ORANGE, 6);
        graphics.endBatch(&batch);
        return;
    }

    batch.drawLine(MARGIN_X, MARGIN_Y + BOARD_HEIGHT, MARGIN_X + BOARD_WIDTH, MARGIN_Y, TFT_ORANGE, 6);   
    graphics.endBatch(&batch);
}

void App_TicTacToe::clear_grid(GBatch_t * batch) {
    batch->fillRectangle(MARGIN_X - 10, MARGIN_Y - 10, BOARD_WIDTH + 10, BOARD_HEIGHT + 10, TFT_WHITE);
}

void App_TicTacToe::print_grid(GBatch_t * batch) {
    batch->drawLine(MARGIN_X, MARGIN_Y + CELL_HEIGHT, MARGIN_X + BOARD_WIDTH, MARGIN_Y + CELL_HEIGHT, TFT_BLACK, 5);
    batch->drawLine(MARGIN_X, MARGIN_Y + 2 * CELL_HEIGHT, MARGIN_X + BOARD_WIDTH, MARGIN_Y + 2 * CELL_HEIGHT, TFT_BLACK, 5);
    batch->drawLine(MARGIN_X + CELL_WIDTH, MARGIN_Y, MARGIN_X + CELL_WIDTH, MARGIN_Y + BOARD_HEIGHT, TFT_BLACK, 5);
    batch->drawLine(MARGIN_X + 2 * CELL_WIDTH, MARGIN_Y, MARGIN_X + 2 * CELL_WIDTH, MARGIN_Y + BOARD_HEIGHT, TFT_BLACK, 5);
}

void App_TicTacToe::print_x(int8_t cell) {
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    int line = (int)floor(cell / 3.0f);
    int column = (cell % 3);

    int X = MARGIN_X + column*CELL_WIDTH + MICRO_MARGIN_X;
    int Y = MARGIN_Y + line*CELL_HEIGHT + MICRO_MARGIN_Y;

    batch.drawLine(X, Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y, TFT_RED, 5);
    batch.drawLine(X, Y + CELL_HEIGHT - 2*MICRO_MARGIN_Y, X + CELL_WIDTH - 2*MICRO_MARGIN_X, Y, TFT_RED, 5);

    graphics.endBatch(&batch);
}

void App_TicTacToe::print_o(int8_t cell) {
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);

    int line = (int)floor(cell / 3.0f);
    int column = (cell % 3);

    int X = MARGIN_X + column*CELL_WIDTH + CELL_WIDTH/2;
    int Y = MARGIN_Y + line*CELL_HEIGHT + CELL_HEIGHT/2;

    batch.drawCircle(X, Y, CELL_WIDTH/2 - MICRO_MARGIN_X, TFT_SKYBLUE, 5);

    graphics.endBatch(&batch);

}

void App_TicTacToe::erase_cell(GBatch_t * batch, int8_t cell) {
    int line = (int)floor(cell / 3.0f);
    int column = (cell % 3);

    int X = MARGIN_X + column*CELL_WIDTH + MICRO_MARGIN_X;
    int Y = MARGIN_Y + line*CELL_HEIGHT + MICRO_MARGIN_Y;

    batch->fillRectangle(X, Y, CELL_WIDTH - 2 * MICRO_MARGIN_X, CELL_HEIGHT - 2 * MICRO_MARGIN_Y, TFT_WHITE);
}

void App_TicTacToe::show_message(const char * message) {
    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
    batch.fillRectangle(0, MARGIN_Y + BOARD_HEIGHT + 40, batch.viewWidth(), 40, TFT_WHITE);
    batch.drawString(batch.viewWidth() / 2, MARGIN_Y + BOARD_HEIGHT + 40 + 20 , message, TFT_BLACK, 2, MC_DATUM);
    graphics.endBatch(&batch);
}
/*
    Data structures
*/

void App_TicTacToe::place_x(int8_t cell) {
    places[cell] = PLACE_X;
}

void App_TicTacToe::place_o(int8_t cell) {
    places[cell] = PLACE_O;
}

/*
    Listeners
*/

int8_t App_TicTacToe::check_click_cells(TouchData data) {
    for(int8_t i = 0; i < 9; i++) {
        if (places[i] != 0) {
            continue;
        }

        if (cell_touch_listeners[i].contains(DEFAULT_VIEWPORT, data)) {
            return i;
        }
    }

    return -1;
}

/*

*/

uint8_t App_TicTacToe::get_winner(uint8_t * layout)
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

    for(uint8_t i = 0; i < 8; i++) {
        uint8_t player = places[winPos[i][0]];
        if (player == 0) {
            continue;
        }

        bool valid = true;
        for(uint8_t j = 1; j < 3; j++) {
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
    canMinimize = false;

    event_group = xEventGroupCreate();
    cell_queue = xQueueCreate(1, sizeof(int8_t));
    
}

void App_TicTacToe::onOpen() {
    ended = false;
    empty = true;

    GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
    batch.fillScreen(TFT_WHITE);

    for(uint8_t i = 0; i < 9; i++) {
        places[i] = 0;
        
        int line = (int)floor(i / 3.0f);
        int column = (i % 3);

        int X = MARGIN_X + column*CELL_WIDTH;
        int Y = MARGIN_Y + line*CELL_HEIGHT;

        cell_touch_listeners[i].x = X;
        cell_touch_listeners[i].y = Y;
        cell_touch_listeners[i].width = CELL_WIDTH;
        cell_touch_listeners[i].height = CELL_HEIGHT;
    }

    print_grid(&batch);

    for(int i = 0; i < 9;i++) {
        erase_cell(&batch, i);
    }

    batch.drawString(batch.viewWidth() / 2, MARGIN_Y / 2, "Tic Tac Toe", TFT_BLACK, 3, MC_DATUM);
    batch.fillRectangle(10, batch.viewHeight() - 40 - 10, batch.viewWidth() - 2*10, 40, TFT_PURPLE);
    batch.drawString(batch.viewWidth() / 2, batch.viewHeight() - 20 - 10 - 2, "RESET", TFT_WHITE, 3, MC_DATUM);

    reset_touch_listener.x = 10;
    reset_touch_listener.y = batch.viewHeight() - 40 - 10;
    reset_touch_listener.width = batch.viewWidth() - 2*10;
    reset_touch_listener.height = 40;

    graphics.endBatch(&batch);
}

void App_TicTacToe::onTick() {
    show_message("Player 1 your turn!");

    int8_t play = get_queued_cell();
    if (play == -1) {
        return;
    }

    place_x(play);
    empty = false;
    print_x(play);

    if (check_winner()) {
        return;
    }

    show_message("Player 2 your turn!");

    play = get_queued_cell();
    if (play == -1) {
        return;
    }

    place_o(play);
    empty = false;
    print_o(play);

    if (check_winner()) {
        return;
    }
}

void App_TicTacToe::onTouchTick() {
    TouchData data = touch.waitRelease();

    if (check_click_reset(data)) {
        return;
    }

    if (ended) {
        return;
    }
    
    int8_t play = check_click_cells(data);
    if (play == -1){
        return;
    }

    xQueueOverwrite(cell_queue, (int8_t *) &play);
    xEventGroupSetBits(event_group, EVENT_DRAW_CELL);
}

void App_TicTacToe::onClose() {
}


int8_t App_TicTacToe::get_queued_cell() {
    EventBits_t bits = xEventGroupWaitBits(event_group, EVENT_MINIMIZE | EVENT_DRAW_CELL, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & EVENT_DRAW_CELL) {
        int8_t play;
        xQueueReceive(cell_queue, &play, 0);
        xEventGroupClearBits(event_group, EVENT_DRAW_CELL);
        
        return play;
    }

    return -1;
}

/*

*/

bool App_TicTacToe::check_winner() {
    uint8_t layout;
    int win = get_winner(&layout);

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

    if (reset_touch_listener.contains(DEFAULT_VIEWPORT, data)) {
        for(int i = 0; i < 9; i++) {
            places[i] = 0;
        }

        empty = true;
        ended = false;

        GBatch_t batch = graphics.beginBatch(DEFAULT_VIEWPORT);
        clear_grid(&batch);
        print_grid(&batch);
        graphics.endBatch(&batch);

        return true;
    }

    return false;
}

