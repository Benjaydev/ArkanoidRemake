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
bool Game::DebugActive = false;

Game::Game() {
    // Initialization
    int screenWidth = 858;
    int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "Arkanoid - Ben Wharton");

    //SetTargetFPS(200);

    // Test sprite functionality
    
    
    background = LoadTexture("Background.png");
   
   
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 13; j++) {
            Brick* brick = new Brick(j * 66, i * 33);
        }
        
    }

    player = new Player(GetScreenWidth()/2, GetScreenHeight() - 100);
    player->IncreasePlayerSize(100);
    
    Ball* ball = new Ball(100, 100);
    //Ball* ball1 = new Ball(600, 100);
    //* ball2 = new Ball(400, 150);





    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose())    
    {
        DeltaTime = timer->RecordNewTime();
        

        Update(DeltaTime);

        PhysicsComponent::GlobalCollisionCheck(DeltaTime);

        Draw();
    }

    delete timer;
    
    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }

    // De-Initialization 
    CloseWindow();
}


void Game::Update(float DeltaTime) {

    // Update objects in world
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Update(DeltaTime);
        if (objects[i]->isWaitingDestroy) {
            delete objects[i];
        }
    }
    /*for (int i : toDestroy) {
        delete objects[i];
    }*/



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

    DrawTexture(background, 0, 0, GetColor(0x333333FF));


    DrawText(("fps: " + std::to_string(timer->fps)).c_str(), 10, GetScreenHeight()-30, 20, BLUE);

    // Draw each object that has a sprite
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Draw();
        
    }


    EndDrawing();
}



Game::~Game() {

}


