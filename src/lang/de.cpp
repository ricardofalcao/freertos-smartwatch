#include "lang/lang.h"


//English

//TicTacToe Text 
PROGMEM const char TTT_name[] = "TicTacToe";
PROGMEM const char TTT_description[] = "Let's play a game!";
PROGMEM const char TTT_reset[] = "Reset";
PROGMEM const char TTT_player1_turn[] = "Player 1 your turn!";
PROGMEM const char TTT_player2_turn[] = "Player 2 your turn!";
PROGMEM const char TTT_draw[] = "It's a draw";
PROGMEM const char TTT_player1_win[] = "Player 1 wins!";
PROGMEM const char TTT_player2_win[] = "Player 2 wins!";

//Metronomer Text
PROGMEM const char Metronome_name[] = "Metronomer";
PROGMEM const char Metronome_description[] = "Time Marker";
PROGMEM const char Metronome_BPM[] = "BPM";

//Clock Text
PROGMEM const char Clock_name[] = "Clock";
PROGMEM const char Clock_description[] = "Shows the current time";
PROGMEM const char Clock_type_analog[] = "Analog";
PROGMEM const char Clock_type_digital[] = "Digital";

//Drawer Text
PROGMEM const char Drawer_name[] = "Drawer";
PROGMEM const char Drawer_description[] = "Shows all installed apps";

//Weather Text
PROGMEM const char Weather_name[] = "Weather";
PROGMEM const char Weather_description[] = "Checks weather through API";

//Monitor Text
PROGMEM const char Monitor_name[] = "Monitor";
PROGMEM const char Monitor_description[] = "Shows the currently active tasks";
PROGMEM const char Monitor_Task[] = "Task";
PROGMEM const char Monitor_CPU[] = "CPU";
PROGMEM const char Monitor_Memory[] = "MEM";

//Pong Text
PROGMEM const char Pong_name[] = "Pong";
PROGMEM const char Pong_description[] = "Classic game Pong";

//Statusbar Text
PROGMEM const char Statusbar_name[] = "Statusbar";
PROGMEM const char Statusbar_description[] = "Shows status and notifications";

//Lang Text
PROGMEM const char Lang_name[] = "Language";
PROGMEM const char Lang_description[] = "Choose the OS language";

//Delayer Lang
PROGMEM const char Delayer_name[] = "Delayer";
PROGMEM const char Delayer_description[] = "Counts time";
PROGMEM const char Delayer_timeout[] = "Time Finished!";


/*

*/

PROGMEM const char * const messages [] = {
    //TicTacToe

    TTT_name,
    TTT_description,
    TTT_reset,
    TTT_player1_turn,
    TTT_player2_turn,
    TTT_draw,
    TTT_player1_win,
    TTT_player2_win,

    //Metronomer
    Metronome_name,
    Metronome_description,
    Metronome_BPM,

    //Clock
    Clock_name,
    Clock_description,
    Clock_type_analog,
    Clock_type_digital,

    //Drawer
    Drawer_name,
    Drawer_description,

    //Weather
    Weather_name,
    Weather_description,

    //Monitor
    Monitor_name,
    Monitor_description,
    Monitor_Task,
    Monitor_CPU,
    Monitor_Memory,

    //Pong
    Pong_name,
    Pong_description,

    //Statusbar
    Statusbar_name,
    Statusbar_description,

    //Lang
    Lang_name,
    Lang_description,

    //Delayer
    Delayer_name,
    Delayer_description,
    Delayer_timeout
};

PROGMEM const Language_t LANG_DE = {
    messages
};