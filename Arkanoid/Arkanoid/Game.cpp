#pragma once
#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include "SpriteComponent.h"
#include "Ball.h"
#include "Brick.h"
using namespace std;

std::vector<Object*> Game::objects = std::vector<Object*>();
int Game::lifetimeObjectCount = 0;
bool Game::DebugActive = true;

Game::Game() {
    // Initialization
    int screenWidth = 858;
    int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "Arkanoid - Ben Wharton");

    SetTargetFPS(200);

    // Test sprite functionality
    
    

   
   
    

    Brick* brick = new Brick(100, 100);

    player = new Player(GetScreenWidth()/2, GetScreenHeight() - 100);

    
    Ball* ball = new Ball(100, 100);

    Ball* ball3 = new Ball(300, 100);
    Ball* ball4 = new Ball(400, 100);



    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose())    
    {
        DeltaTime = timer->RecordNewTime();

        if (DeltaTime == 0) {
            std::cout << DeltaTime << std::endl;
        }
        

        Update(DeltaTime);

        PhysicsComponent::GlobalCollisionCheck(DeltaTime);

        Draw();
    }

    delete timer;
    delete player;
    delete ball;
    delete brick;
    delete ball3;
    delete ball4;


    // De-Initialization 
    CloseWindow();
}


void Game::Update(float DeltaTime) {

    // Update objects in world
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Update(DeltaTime);

    }

    if (IsKeyDown(KEY_A))
    {
        player->physics->Accelerate(-1);
    }
    if (IsKeyDown(KEY_D))
    {
        player->physics->Accelerate(1);
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
        objects[i]->Draw();
        
    }


    EndDrawing();
}



Game::~Game() {

}


