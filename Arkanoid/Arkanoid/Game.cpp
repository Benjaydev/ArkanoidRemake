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

    gameBackground = LoadTexture("Background.png");
    StartMainMenu();
   //StartGame(0);
    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose())    
    {
        DeltaTime = timer->RecordNewTime();
        Update(DeltaTime);

        
       

        Draw();
    }

    delete timer;
    ResetGameObjects();
    

    // De-Initialization 
    CloseWindow();
}


void Game::ResetGameObjects() {

    if (mainMenu != nullptr) {
        delete mainMenu;
        mainMenu = nullptr;
    }
    if (pauseMenu != nullptr) {
        delete pauseMenu;
        pauseMenu = nullptr;
    }

    // Delete all objects
    for (int i = 0; i < objects.size(); i++) {
        storeDestroy.push_back(std::bind(&Object::DeleteSelf, objects[i]));
    }

    DestroyStoredAwaiting();

    
}
void Game::DestroyStoredAwaiting()
{
    for (int del = 0; del < storeDestroy.size(); del++) {
        storeDestroy[del]();
    }
    storeDestroy.clear();
    storeDestroy.shrink_to_fit();

    objects.shrink_to_fit();
}




void Game::StartMainMenu()
{
    ResetGameObjects();
    IsGamePaused = true;

    mainMenu = new MainMenu();
    mainMenu->startGameButton->AssignCallMethod(std::bind(&Game::StartGame, this, 0));
    mainMenu->levelEditButton->AssignCallMethod(std::bind(&Game::StartLevelEditor, this));
}

void Game::StartLevelEditor()
{
    ResetGameObjects();
    IsGamePaused = true;

    levelEditor = new LevelEditor();
    backgroundColour = levelEditor->map.mapStruct.backgroundColour;

}



void Game::StartGame(int index) {
    IsGamePaused = false;
    ResetGameObjects();
    

    Map map = Map();
    backgroundColour = map.mapStruct.backgroundColour;
    map.LoadMap(index);
    map.GenerateMap();
    
    player = new Player(GetScreenWidth() / 2, GetScreenHeight() - 100);

    Ball* ball = new Ball(player->physics->globalTransform.m8, player->physics->globalTransform.m9 - 50);


}

void Game::TogglePauseMenu() {
    IsGamePaused = !IsGamePaused;

    // PROBLEM
    if (pauseMenu != nullptr) {
        delete pauseMenu;
        pauseMenu = nullptr;
   }
    if (IsGamePaused) {
        pauseMenu = new PauseMenu(GetScreenWidth() / 2, GetScreenHeight() / 2);

        pauseMenu->resumeButton->AssignCallMethod(std::bind(&Game::TogglePauseMenu, this));
        pauseMenu->mainmenuButton->AssignCallMethod(std::bind(&Game::StartMainMenu, this));
    }
}

void Game::Update(float DeltaTime) {
    
    // Update objects in world
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Update(DeltaTime);
        // If the update reslts in a reset in objects
        if (i >= objects.size()) { break; }

        // Destroy all objects that are awaiting
        if (objects[i]->isWaitingDestroy) {
            storeDestroy.push_back(std::bind(&Object::DeleteSelf, objects[i]));
            continue;
        }
    }
    DestroyStoredAwaiting();
    


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
    
    DrawTexture(gameBackground, 0, 0, GetColor(backgroundColour));


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


