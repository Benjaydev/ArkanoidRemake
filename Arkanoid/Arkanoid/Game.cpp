#pragma once
#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <string>
#include "SpriteComponent.h"
#include "Ball.h"
#include "Brick.h"
#include "Map.h"
#include <functional>

using namespace std;

std::vector<Object*> Game::objects = std::vector<Object*>();
int Game::lifetimeObjectCount = 0;
bool Game::DebugActive = false;
bool Game::IsGamePaused = false;

Game::Game() {
    // Initialization
    int screenWidth = 858;
    int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "Arkanoid - Ben Wharton");

    //SetTargetFPS(30);

    StartGame(0);
    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose())    
    {
        DeltaTime = timer->RecordNewTime();
        Update(DeltaTime);

        
       

        Draw();
    }

    delete timer;
    delete pauseMenu;
    ResetGameObjects();
    

    // De-Initialization 
    CloseWindow();
}


void Game::ResetGameObjects() {
    // Delete all objects
    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
}




void Game::StartGame(int index) {
    background = LoadTexture("Background.png");

    Map map = Map();

    //map.LoadMap(index);
    //map.GenerateMap();
    
    player = new Player(GetScreenWidth() / 2, GetScreenHeight() - 100);

    Ball* ball = new Ball(player->physics->globalTransform.m8, player->physics->globalTransform.m9 - 50);


}

void Game::TogglePauseMenu() {
    IsGamePaused = !IsGamePaused;

    delete pauseMenu;
    pauseMenu = nullptr;
    if (IsGamePaused) {
        pauseMenu = new PauseMenu(GetScreenWidth() / 2, GetScreenHeight() / 2);

        pauseMenu->resumeButton->AssignCallMethod(std::bind(&Game::TogglePauseMenu, this));
    }
}


void Game::Update(float DeltaTime) {
    
    // Update objects in world
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Update(DeltaTime);
        if (objects[i]->isWaitingDestroy) {
            delete objects[i];
            continue;
        }

        if (objects[i]->tag == "UI" && objects[i]->physics->collider != nullptr) {
            if (objects[i]->physics->collider->Overlaps(GetMousePosition())) {

                ((UIButton*)objects[i])->OnHover();
                // Button Click
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    ((UIButton*)objects[i])->OnClick();
                }
            }
        }
        

    }
    

    if (!IsGamePaused) {

        

        PhysicsComponent::GlobalCollisionCheck(DeltaTime);

        if (IsKeyDown(KEY_A))
        {
            player->physics->Accelerate(-1);
        }
        if (IsKeyDown(KEY_D))
        {
            player->physics->Accelerate(1);
        }
        if (IsKeyPressed(KEY_SPACE)) {
            Ball* ball = new Ball(player->physics->globalTransform.m8, player->physics->globalTransform.m9 - 50);
        }

    }


    if (IsKeyPressed(KEY_Q)) {
        TogglePauseMenu();
    }

  
}

void Game::Draw()
{
    // Draw
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawTexture(background, 0, 0, GetColor(0x333333FF));


    DrawText(("fps: " + std::to_string(timer->fps)).c_str(), 10, GetScreenHeight()-30, 20, BLUE);
    DrawText("Pause: Q", GetScreenWidth() - MeasureText("Pause: Q", 20)-10, GetScreenHeight() - 30, 20, BLUE);

    // Draw each object that has a sprite
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Draw();
        
    }


    EndDrawing();
}



Game::~Game() {

}


