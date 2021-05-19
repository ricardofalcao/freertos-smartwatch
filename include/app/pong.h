#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "app/app.h"


class App_Pong : public App {
    private:
        int tick = 0;
        
        int playerx = DEFAULT_VIEWPORT.width / 2, tplayerx = DEFAULT_VIEWPORT.width / 2;

        int botx = DEFAULT_VIEWPORT.width / 2;

        int ballx = DEFAULT_VIEWPORT.width / 2, bally = DEFAULT_VIEWPORT.height / 2;

        char _bot_score[3];
        char _player_score[3];

    public:
        App_Pong();

        void onOpen() override;    

        void onTick() override;   

        void onClose() override;   
    
    private:
        void print_top_bar(GBatch_t * batch, int previous_x, int new_x);

        void print_bottom_bar(GBatch_t * batch, int previous_x, int new_x);

        void print_field_lines(GBatch_t * batch, uint32_t color);

        void print_ball(GBatch_t * batch, int x_center, int y_center,int previous_x, int previous_y, uint32_t color);

        void print_score(GBatch_t * batch, uint32_t color);
};