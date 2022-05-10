#pragma once
#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include "SpriteComponent.h"
#include "TestObject.h"
using namespace std;

std::vector<Object*> Game::objects = std::vector<Object*>();
int Game::lifetimeObjectCount = 0;

Game::Game() {
    // Initialization
    int screenWidth = 858;
    int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "Arkanoid - Ben Wharton");

    //SetTargetFPS(2);

    // Test sprite functionality
    TestObject* to = new TestObject(700, GetScreenHeight() - 100, cType::Circle);
    TestObject* to2 = new TestObject(100, GetScreenHeight() - 100, cType::Circle);
    TestObject* to3 = new TestObject(GetScreenWidth() / 2, 100, cType::Circle);

    //TestObject* to3 = new TestObject(100, 200, cType::Circle);
   // TestObject* to4 = new TestObject(500, 225, cType::Circle);
    
    //TestObject* to5 = new TestObject(100, 300, cType::Rectangle);
    //TestObject* to6 = new TestObject(500, 325, cType::Rectangle);
    

    player = new Player(GetScreenWidth()/2, GetScreenHeight() - 100);

    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose())    
    {
        DeltaTime = timer->RecordNewTime();

        to3->parent->physics->velocity->y = 100;
        Update(DeltaTime);

        PhysicsComponent::GlobalCollisionCheck(DeltaTime);

        Draw();
    }

    delete timer;
    delete player;
    delete to;
    delete to2;
    delete to3;

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


