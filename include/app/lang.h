#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "time.h"
#include "app/app.h"
#include "touch.h"

class App_Lang : public App {
    private:
        TFT_eSprite icon_de = TFT_eSprite(&tft);
        TFT_eSprite icon_en = TFT_eSprite(&tft);
        TFT_eSprite icon_pt = TFT_eSprite(&tft);

        RectangleTouchListener touch_de;
        RectangleTouchListener touch_en;
        RectangleTouchListener touch_pt;

    public:
        App_Lang();

        void onOpen() override;   

        void onResume() override;  

        void onTouchTick() override;   

        void onClose() override;   

    private:
        void draw_langs(GBatch_t * batch);

};
