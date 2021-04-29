#pragma once

#include <Arduino.h>

#include "app/app.h"


class App_Pong : public App {

    public:
        App_Pong();

        void onOpen() override;   

        void onTick() override;   

        void onClose() override;   
    
    private:
        void print_top_bar(int place_x);

        void print_bottom_bar(int place_x);

        void print_dashed();

        void print_ball(int x_center, int y_center);

        void erase_top_bar(int place_x);

        void erase_bottom_bar(int place_x);

        void erase_ball(int x_center, int y_center);
};