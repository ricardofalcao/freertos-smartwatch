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
PROGMEM const char Metronome_BPM[] = "BPM";

//Clock Text
PROGMEM const char Clock_name[] = "Relógio";
PROGMEM const char Clock_description[] = "Mostra o tempo atual";
PROGMEM const char Clock_type_analog[] = "Analógico";
PROGMEM const char Clock_type_digital[] = "Digital";

//Alarm Text
PROGMEM const char Alarm_name[] = "Alarme";
PROGMEM const char Alarm_description[] = "Horários e eventos";

//Drawer Text
PROGMEM const char Drawer_name[] = "Gaveta de aplicações";
PROGMEM const char Drawer_description[] = "Mostra todas as aplicações instaladas";

//Weather Text
PROGMEM const char Weather_name[] = "Metereologia";
PROGMEM const char Weather_description[] = "Consulta a meteorologia através duma API";

//Monitor Text
PROGMEM const char Monitor_name[] = "Gestor";
PROGMEM const char Monitor_description[] = "Gere as tarefas ativas no momento";
PROGMEM const char Monitor_Task[] = "Tarefa";
PROGMEM const char Monitor_CPU[] = "CPU";
PROGMEM const char Monitor_Memory[] = "MEM";

//Pong Text
PROGMEM const char Pong_name[] = "Pong";
PROGMEM const char Pong_description[] = "O jogo classico de Pong";

//Statusbar Text
PROGMEM const char Statusbar_name[] = "Barra de Estado";
PROGMEM const char Statusbar_description[] = "Mostra o estado e notificações";

//Lang Text
PROGMEM const char Lang_name[] = "Língua";
PROGMEM const char Lang_description[] = "Escolhe a língua do SO";

//Delayer Lang
PROGMEM const char Delayer_name[] = "Temporizador";
PROGMEM const char Delayer_description[] = "Contagem decrescente";
PROGMEM const char Delayer_timeout[] = "Acabou o Tempo!";

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
    Metronomer_name,
    Metronomer_description,
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
    
    //
    Delayer_name,
    Delayer_description,
    Delayer_timeout

};

PROGMEM const Language_t LANG_PT = {
    messages
};