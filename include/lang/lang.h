#pragma once

#include "sdkconfig.h"
#include <Arduino.h>

typedef enum 
{   
    //TicTacToe
    MSG_TTT_NAME = 0,
    MSG_TTT_DESCRIPTION,
    MSG_TTT_RESET,
    MSG_TTT_PLAYER1_NUM,
    MSG_TTT_PLAYER2_NUM,
    MSG_TTT_DRAW,
    MSG_TTT_PLAYER1_WIN,
    MSG_TTT_PLAYER2_WIN,

    //Metronomer
    MSG_METRONOMER_NAME,
    MSG_METRONOMER_DESCRIPTION,
    MSG_METRONOME_BPM,

    //Clock
    MSG_CLOCK_NAME,
    MSG_CLOCK_DESCRIPTION,
    MSG_CLOCK_TYPE_ANALOG,
    MSG_CLOCK_TYPE_DIGITAL,
    
    //Drawer
    MSG_DRAWER_NAME,
    MSG_DRAWER_DESCRIPTION,
    
    //Weather
    MSG_WEATHER_NAME,
    MSG_WEATHER_DESCRIPTION,

    //Monitor
    MSG_MONITOR_NAME,
    MSG_MONITOR_DESCRIPTION,
    MSG_MONITOR_TASK,
    MSG_MONITOR_CPU,
    MSG_MONITOR_MEM,

    //Pong
    MSG_PONG_NAME,
    MSG_PONG_DESCRIPTION,

    //Statusbar
    MSG_STATUSBAR_NAME,
    MSG_STATUSBAR_DESCRIPTION,

    //Lang
    MSG_LANG_NAME,
    MSG_LANG_DESCRIPTION,

    //Delayer
    MSG_DELAYER_NAME,
    MSG_DELAYER_DESCRIPTION,
    MSG_DELAYER_TIMEOUT,

    MSG_NUM

} Message_t;


typedef struct {

    const char * const * messages;

} Language_t;


extern const Language_t LANG_EN;
extern const Language_t LANG_PT;
extern const Language_t LANG_DE;

/*

*/

class Lang {
    private:
        const Language_t * active_language = &LANG_EN;
    
    public:

        const Language_t * getLanguage();

        void setLanguage(const Language_t * language);

        const char * get(Message_t message);
};

extern Lang lang;