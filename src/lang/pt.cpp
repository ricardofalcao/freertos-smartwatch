#include "lang/lang.h"


//English

//TicTacToe Text 
PROGMEM const char TTT_name[] = "Jogo do Galo";
PROGMEM const char TTT_description[] = "Vamos Jogar!";
PROGMEM const char TTT_reset[] = "Reiniciar";
PROGMEM const char TTT_player1_turn[] = "Jogador 1 é a tua vez!";
PROGMEM const char TTT_player2_turn[] = "Jogador 2 é a tua vez!";
PROGMEM const char TTT_draw[] = "É um empate!";
PROGMEM const char TTT_player1_win[] = "Vitória do Jogador 1!";
PROGMEM const char TTT_player2_win[] = "Vitória do Jogador 2!";

//Metronomer Text
PROGMEM const char Metronomer_name[] = "Metrónomo";
PROGMEM const char Metronomer_description[] = "Marcador de tempo";
PROGMEM const char BPM[] = "BPM";

//Clock Text
PROGMEM const char Clock_name[] = "Relógio";
PROGMEM const char Metronomer_description[] = "Mostra o tempo atual";

//Alarm Text
PROGMEM const char Alarm_name[] = "Alarme";
PROGMEM const char Alarm_description[] = "Horários e eventos";

//Drawer Text
PROGMEM const char Drawer_name[] = "Gaveta de aplicações";
PROGMEM const char Drawer_description[] = "Mostra todas as aplicações instaladas";

//Weather Text
PROGMEM const char Weather_name[] = "Metereologia";
PROGMEM const char Weather_description[] = "Consulta a meteorologia através duma API";

/*

*/

PROGMEM const char * const messages [] = {
    //TicTacToe
    TTT_name,
    TTT_reset,
    TTT_player1_turn,
    TTT_player2_turn,
    TTT_draw,
    TTT_player1_win,
    TTT_player2_win,

    //Metronomer
    Metronomer_name,
    Metronomer_description,
    BPM,

    //Clock
    Clock_name,
    Metronomer_description,
    
    //Alarm
    Alarm_name,
    Alarm_description,

    //Drawer
    Drawer_name,
    Drawer_description,

    //Weather
    Weather_name,
    Weather_description

};

PROGMEM const Language_t LANG_PT = {
    messages
};