#pragma once
#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include "SpriteComponent.h"
#include "Ball.h"
#include "Brick.h"
#include "Map.h"
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
   
    Map map = Map();
    
    /*
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 13; j++) {

            int index = (i * 13) + j;
            map.AddBrick(new Brick(j * 66, i * 33, 1, 0x00FFFFFF), index);

        }

    }
    map.mapName = "Test Level";

    map.SaveMap();*/

    std::cout << map.GetMapCount() << std::endl;
    std::cout << map.GetMapName(0) << std::endl;
    map.LoadMap(0);
    map.GenerateMap();


    player = new Player(GetScreenWidth()/2, GetScreenHeight() - 100);
    //player->IncreasePlayerSize(100);
    
    Ball* ball = new Ball(player->physics->globalTransform.m8, player->physics->globalTransform.m9 - 50);





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
    if (IsKeyPressed(KEY_SPACE)) {
        Ball* ball = new Ball(player->physics->globalTransform.m8, player->physics->globalTransform.m9-50);
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


