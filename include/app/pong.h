#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "app/app.h"

class App_Pong : public App {
    private:
        int tick = 0;
        int deadTime = 0;

        int bot_score;
        float bot_max_speed = 5;
        int bot_tick_ms = 10;

        int player_score;
        
        float playerx, tplayerx;
        float botx;

        float ballx, bally;
        float ballvx, ballvy;

        char _bot_score[12];
        char _player_score[12];

        uint8_t state;

    public:
        App_Pong();

        void onOpen() override;    

        void onTick() override;   

        void onClose() override;   
    
    private:
        void setup_game();

        void reset_game();

        void update_game();

        void print_top_bar(GBatch_t * batch, int previous_x, int new_x);

        void print_bottom_bar(GBatch_t * batch, int previous_x, int new_x);

        void print_middle_lines(GBatch_t * batch, uint32_t color);

        void print_field_lines(GBatch_t * batch, uint32_t color);

        void print_ball(GBatch_t * batch, int x_center, int y_center, int previous_x_center, int previous_y_center);

        void print_score(GBatch_t * batch, uint32_t color);
};