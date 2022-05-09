#pragma once
#include "Game.h"


int main(int argc, char* argv[])
{
    Game* game = new Game();
    delete game;
    game = nullptr;

    return 0;
}