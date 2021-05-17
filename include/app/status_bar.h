#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

#include "app/app.h"
#include "notifications.h"

class App_Statusbar : public App {
    private:
        TFT_eSprite icon_wifi = TFT_eSprite(&tft);

        int16_t current_notification_cursor = 0;
        TFT_eSprite notification_sprite = TFT_eSprite(&tft);
        
        float last_battery;

    public:
        App_Statusbar();

        void onOpen() override;   

        void onClose() override;   

        void onTick() override;   

};
