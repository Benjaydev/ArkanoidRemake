#pragma once
#include "Game.h"


int main(int argc, char* argv[])
{
    // Create game
    Game* game = new Game();
    // Delete on finish
    delete game;
    game = nullptr;

    // Close program
    return 0;
}