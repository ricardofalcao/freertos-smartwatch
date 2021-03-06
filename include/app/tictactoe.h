#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "touch.h"
#include "app/app.h"

#define CELL_TOUCH_LISTENERS 9

class App_TicTacToe : public App {
    private:
        uint8_t places[9];

        bool ended = false;
        bool empty = true;

        QueueHandle_t cell_queue;

        RectangleTouchListener cell_touch_listeners[CELL_TOUCH_LISTENERS];
        RectangleTouchListener reset_touch_listener;

    public:
        App_TicTacToe();

        void onOpen() override;   

        void onTick() override;   

        void onTouchTick() override;   

        void onClose() override;   

    private:
        bool check_winner();

        bool check_click_reset(TouchData data);

        void print_winning_line(int8_t layout);

        void clear_grid(GBatch_t * batch);

        void print_grid(GBatch_t * batch);

        uint8_t get_winner(uint8_t * layout);

        void place_x(int8_t cell);

        void place_o(int8_t cell);

        void print_x(int8_t cell);

        void print_o(int8_t cell);

        void erase_cell(GBatch_t * batch, int8_t cell);

        void show_message(const char * message);

        int8_t check_click_cells(TouchData data);

        int8_t get_queued_cell();

};