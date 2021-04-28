#pragma once

#include <Arduino.h>

#include "touch.h"
#include "app/app.h"

class App_TicTacToe : public App {
    private:
        bool ended = false;
        bool empty = true;

    public:
        App_TicTacToe();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   

    private:
        bool check_winner();

        bool check_click_reset(TouchData data);

        void print_winning_line(int8_t layout);

        void clear_grid();

        void print_grid();

        void print_x(int8_t cell);

        void print_o(int8_t cell);

        void erase_cell(int8_t cell);

        void show_message(const char * message);

};