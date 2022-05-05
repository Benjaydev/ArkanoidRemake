#pragma once
#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include "SpriteObject.h"
#include "Object.h"


std::list<Object> Game::objects = std::list<Object>();


Game::Game() {



    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Arkanoid - Ben Wharton");

    //SetTargetFPS(60);

    
    SpriteObject* test = new SpriteObject();
    test->Load((char*)"TestImage.png");

    test->AddToGameWorld();

    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose())    
    {
        DeltaTime = timer->RecordNewTime();

        Update(DeltaTime);

        Draw();
    }

    delete timer;

    // De-Initialization 
    CloseWindow();
}


void Game::Update(float DeltaTime) {
    // Update
    if (IsKeyPressed(KEY_LEFT))
    {
        DrawText("LEFT", 100, 50, 20, BLUE);
    }

    if (IsKeyPressed(KEY_RIGHT))
    {
        DrawText("RIGHT", 100, 120, 20, BLUE);
    }
}

void Game::Draw()
{
    // Draw
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText(("fps: " + std::to_string(timer->fps)).c_str(), 10, 10, 20, BLUE);


    for (Object obj : objects) {
        ((SpriteObject*)&obj)->Draw();
    }


    EndDrawing();
}



Game::~Game() {

}


