#include "app/tictactoe.h"

#include "graphics.h"

App_TicTacToe::App_TicTacToe() : App("TicTacToe", "Let's play a game") {
    priority = 3;
    stack_depth = 1024;
}

void App_TicTacToe::onOpen() {
    Serial.println("[Clock] OPEN");
}

void App_TicTacToe::onTick() {
    Serial.println("[TicTacToe] Tick");
    
}

void App_TicTacToe::onClose() {
    Serial.println("[TicTacToe] Closed");
}