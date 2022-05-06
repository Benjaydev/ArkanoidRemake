#pragma once
#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include "SpriteComponent.h"
#include "Object.h"
using namespace std;

std::vector<Object*> Game::objects = std::vector<Object*>();
int Game::lifetimeObjectCount = 0;

Game::Game() {
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Arkanoid - Ben Wharton");

    //SetTargetFPS(60);

    // Test sprite functionality
    Object* parent = new Object();
    parent->LoadSprite((char*)"TestImage.png");
    parent->sprite->SetScale(0.25);

    parent->physics->SetPosition(300, 200);

    Object* child = new Object();
    child->LoadSprite((char*)"TestImage.png");
    child->sprite->SetScale(0.5);
    std::cout << child->sprite->GetWidth() << std::endl;

    
    //child->physics->SetPosition(100, 100);

    parent->AddChild(child);
    child->physics->SetPosition(-(child->sprite->GetWidth() / 2), -(child->sprite->GetHeight() / 2));
    parent->AddToGameWorld();


    //parent->physics->SetRotation(0 * DEG2RAD);

    

    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose())    
    {
        DeltaTime = timer->RecordNewTime();

        
        Update(DeltaTime);

        //parent->physics->Accelerate(1);
        parent->physics->Rotate(0.0005 * DEG2RAD);

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


