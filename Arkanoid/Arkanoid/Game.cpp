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
    int screenWidth = 800;
    int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Arkanoid - Ben Wharton");

    //SetTargetFPS(2);

    // Test sprite functionality
    TestObject* to = new TestObject(100, 100, cType::Rectangle);
    TestObject* to2 = new TestObject(500, 125, cType::Rectangle); 
    TestObject* to3 = new TestObject(300, 125, cType::Rectangle); 

    
    //TestObject* to3 = new TestObject(100, 200, cType::Circle);
   // TestObject* to4 = new TestObject(500, 225, cType::Circle);
    
    //TestObject* to5 = new TestObject(100, 300, cType::Rectangle);
    //TestObject* to6 = new TestObject(500, 325, cType::Rectangle);


    
    
    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose())    
    {
        DeltaTime = timer->RecordNewTime();

        
        Update(DeltaTime);

        to->parent->physics->Accelerate(1);
        to2->parent->physics->Accelerate(-1);
        to3->parent->physics->Accelerate(-1);

       // to3->parent->physics->Accelerate(1);
       // to4->parent->physics->Accelerate(-1);

       // to5->parent->physics->Accelerate(1);
       // to6->parent->physics->Accelerate(-1);

        PhysicsComponent::GlobalCollisionCheck();

        Draw();
    }

    delete timer;
    delete to;
    delete to2;
    delete to3;
   // delete to4;
   // delete to5;
   // delete to6;

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


