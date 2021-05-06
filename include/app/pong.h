#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "app/app.h"


class App_Pong : public App {
    private:
        int tick = 0;
        int playerx = VIEW_WIDTH / 2, tplayerx = VIEW_WIDTH / 2;
        int botx = VIEW_WIDTH / 2;
        char _bot_score[3];
        char _player_score[3];

    public:
        App_Pong();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   
    
    private:
        void print_top_bar(int previous_x, int new_x);

        void print_bottom_bar(int previous_x, int new_x);

        void print_field_lines(uint32_t color);

        void print_ball(int x_center, int y_center, uint32_t color);

        void print_score(uint32_t color);
};