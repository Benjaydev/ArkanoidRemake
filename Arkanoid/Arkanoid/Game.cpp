#pragma once
#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include "SpriteComponent.h"
#include "Object.h"


std::vector<Object*> Game::objects = std::vector<Object*>();


Game::Game() {



    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Arkanoid - Ben Wharton");

    //SetTargetFPS(60);

    // Test sprite functionality
    Object* ob = new Object();
    ob->LoadSprite((char*)"TestImage.png");
    ob->sprite->textureScale = 0.5;
    ob->AddToGameWorld();


    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose())    
    {
        DeltaTime = timer->RecordNewTime();

        ob->physics->Accelerate(1);

        Update(DeltaTime);

        Draw();
    }

    delete timer;

    // De-Initialization 
    CloseWindow();
}


void Game::Update(float DeltaTime) {

    // Update objects in world
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Update(DeltaTime);

    }

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

    // Draw each object that has a sprite
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->hasSprite) {
            objects[i]->Draw();
        }
        
    }


    EndDrawing();
}



Game::~Game() {

}


