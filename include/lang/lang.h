#pragma once

#include <Arduino.h>

#include "app/app.h"

typedef enum 
{   
    //TicTacToe
    TTT_NAME = 0,
    TTT_DESCRIPTION,
    TTT_RESET,
    TTT_PLAYER1_NUM,
    TTT_PLAYER2_NUM,
    TTT_DRAW,
    TTT_PLAYER1_WIN,
    TTT_PLAYER2_WIN,

    //Metronomer
    METRONOMER_NAME,
    METRONOMER_DESCRIPTION,
    BPM,

    //Clock
    CLOCK_NAME,
    CLOCK_DESCRIPTION,

    //Alarm
    ALARM_NAME,
    ALARM_DESCRIPTION,

    //Drawer
    DRAWER_NAME,
    DRWAER_DESCRIPTION,

    //Weather
    WEATHER_NAME,
    WEATHER_DESCRIPTION,


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

        void setLanguage(const Language_t * language);

        const char * get(Message_t message);
};

extern Lang lang;