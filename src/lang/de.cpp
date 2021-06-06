#include "lang/lang.h"


//German

//TicTacToe Text 
PROGMEM const char TTT_name[] = "TicTacToe";
PROGMEM const char TTT_description[] = "Lass uns ein Spiel spielen!";
PROGMEM const char TTT_reset[] = "Neu Start";
PROGMEM const char TTT_player1_turn[] = "Soieler 1 an der Reihe!";
PROGMEM const char TTT_player2_turn[] = "Spieler 2 an der Reihe!";
PROGMEM const char TTT_draw[] = "Unenschieden!";
PROGMEM const char TTT_player1_win[] = "Spieler 1 gewinnt!";
PROGMEM const char TTT_player2_win[] = "Spieler 2 gewinnt!";

//Metronomer Text
PROGMEM const char Metronome_name[] = "Metronom";
PROGMEM const char Metronome_description[] = "Zeit Marker";
PROGMEM const char Metronome_BPM[] = "BPM";

//Clock Text
PROGMEM const char Clock_name[] = "Uhr";
PROGMEM const char Clock_description[] = "Actuelle Uhrzeit";
PROGMEM const char Clock_type_analog[] = "Analog";
PROGMEM const char Clock_type_digital[] = "Digital";

//Drawer Text
PROGMEM const char Drawer_name[] = "Drawer";
PROGMEM const char Drawer_description[] = "Alle installierten Apps anzeigen";

//Weather Text
PROGMEM const char Weather_name[] = "Wetter";
PROGMEM const char Weather_description[] = "Wetter überprüfen mit API";

//Monitor Text
PROGMEM const char Monitor_name[] = "Monitor";
PROGMEM const char Monitor_description[] = "Aktive Tasks anzeigen";
PROGMEM const char Monitor_Task[] = "Task";
PROGMEM const char Monitor_CPU[] = "CPU";
PROGMEM const char Monitor_Memory[] = "MEM";

//Pong Text
PROGMEM const char Pong_name[] = "Pong";
PROGMEM const char Pong_description[] = "Klassisches Pong Spiel";

//Statusbar Text
PROGMEM const char Statusbar_name[] = "Statusanzeige";
PROGMEM const char Statusbar_description[] = "Status und Benachrichtigungen anzeigen";

//Lang Text
PROGMEM const char Lang_name[] = "Sprache";
PROGMEM const char Lang_description[] = "Auswahl der OS Sprache";

//Delayer Lang
PROGMEM const char Delayer_name[] = "Delayer";
PROGMEM const char Delayer_description[] = "Misst die Zeit";
PROGMEM const char Delayer_timeout[] = "Messung erfolgreich!";


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