#pragma once

#include <Arduino.h>

#include "app/app.h"

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
    MSG_BPM,

    //Clock
    MSG_CLOCK_NAME,
    MSG_CLOCK_DESCRIPTION,

    //Alarm
    MSG_ALARM_NAME,
    MSG_ALARM_DESCRIPTION,

    //Drawer
    MSG_DRAWER_NAME,
    MSG_DRAWER_DESCRIPTION,

    //Weather
    MSG_WEATHER_NAME,
    MSG_WEATHER_DESCRIPTION,


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