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
#include "Powerup.h"

using namespace std;

Player* Game::player;
std::vector<Object*> Game::objects = std::vector<Object*>();
int Game::lifetimeObjectCount = 0;
int Game::ActiveBalls = 0;
int Game::BrickCount = 0;
bool Game::DebugActive = false;
bool Game::IsGamePaused = false;
bool Game::IsEditing = false;
bool Game::HasGameStarted = false;
bool Game::IsStoryMode = false;
Game* Game::ThisGame = nullptr;
bool Game::CloseGame = false;

Vector4 Game::WorldBorders = { 38, 33, 886, 883 };



Game::Game() {

    // Initialization
    //int screenWidth = 858;
    //int screenHeight = 850;
    
    int screenWidth = 924;
    int screenHeight = 883;
    InitWindow(screenWidth, screenHeight, "Arkanoid - Ben Wharton");
    //InitAudioDevice();

    //SetTargetFPS(30);
    ThisGame = this;
    SetTraceLogLevel(LOG_NONE);
    if (DebugActive) {
        SetTraceLogLevel(LOG_ALL);
    }

    

    gameBackground = LoadTexture("Images/Background.png");
    gameBorder = LoadTexture("Images/Border.png");
    paddleLife = LoadTexture("Images/Paddle.png");
    StartMainMenu();

    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose() && !CloseGame)    
    {
        DeltaTime = timer->RecordNewTime();
        
        Update(DeltaTime);
        Draw();
    }

    delete timer;
    
    UnloadTexture(gameBorder);
    UnloadTexture(gameBackground);

    UnloadSound(Powerup::PowerupSound);
    UnloadSound(Ball::BrickHitSound);
    UnloadSound(Ball::PaddleHitSound);

    ResetGameObjects();
    
    CloseAudioDevice();

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
    if (levelEditor != nullptr) {
        delete levelEditor;
        levelEditor = nullptr;
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
    Map::saveLocation = "Saves.txt";
    ResetGameObjects();
    IsStoryMode = false;
    IsGamePaused = true;
    IsEditing = false;

    backgroundColour = 0x333333FF;
    mainMenu = new MainMenu();
    mainMenu->levelEditButton->AssignCallMethod(std::bind(&Game::StartLevelEditor, this));
    mainMenu->storyModeButton->AssignCallMethod(std::bind(&Game::StartGameStory, this));
}

void Game::StartLevelEditor()
{
    ResetGameObjects();
    IsGamePaused = true;
    IsEditing = true;

    levelEditor = new LevelEditor();
    levelEditor->mainMenuButton->AssignCallMethod(std::bind(&Game::StartMainMenu, this));

}



void Game::StartGame(int index) {
    currentMapIndex = index;
    IsGamePaused = false;
    IsEditing = false;
    HasGameStarted = false;

    ResetGameObjects();
    
    // Load map
    Map map = Map();
    map.LoadMap(index);
    map.GenerateMap();
    backgroundColour = map.mapStruct.backgroundColour;

    // Create player
    player = new Player(GetScreenWidth() / 2, GetScreenHeight() - 100);
    player->IncreasePlayerSize(20);

    // Create starting ball
    new Ball(player->physics->globalTransform.m8, player->physics->globalTransform.m9 - 30);


}

void Game::StartGameStory()
{
    IsStoryMode = true;
    Map::saveLocation = "StorySaves.txt";
    StartGame(0);
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

        if (CloseGame) {
            return;
        }
        // If the update reslts in a reset in objects
        if (i >= objects.size()) { break; }

        // Destroy all objects that are awaiting
        if (objects[i]->isWaitingDestroy) {
            storeDestroy.push_back(std::bind(&Object::DeleteSelf, objects[i]));
            continue;
        }
    }
    DestroyStoredAwaiting();
    
    if (IsEditing) {
        backgroundColour = levelEditor->map.mapStruct.backgroundColour;
        return;
    }

    if (!IsGamePaused) {
        if (HasGameStarted) {
            PhysicsComponent::GlobalCollisionCheck(DeltaTime);

            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            {
                player->physics->Accelerate(-1);
            }
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            {
                player->physics->Accelerate(1);
            }

            // Win case
            if (BrickCount == 0) {
                if (IsStoryMode) {
                    Map map = Map();

                    if (currentMapIndex + 1 < map.GetMapCount()) {
                        StartGame(currentMapIndex + 1);
                    }
                    else {
                        StartMainMenu();
                        return;
                    }
                    
                }
                else {
                    StartMainMenu();
                    return;
                }
            }
            if (ActiveBalls == 0) {
                player->lives--;
                // Gameover
                if (player->lives == -1) {
                    StartMainMenu();
                    return;
                }

                // Spawn new ball
                HasGameStarted = false;
                player->physics->SetPosition(GetScreenWidth() / 2, GetScreenHeight() - 100);
                new Ball(player->physics->globalTransform.m8, player->physics->globalTransform.m9 - 30);
            }
        }
        
        
        if (IsKeyDown(KEY_SPACE)) {
            if (!HasGameStarted) {
                HasGameStarted = true;
            }
        }
    }


    if (mainMenu == nullptr) {
        if (IsKeyPressed(KEY_Q)) {
            TogglePauseMenu();
        }
    }
    

  
}
void Game::Draw()
{
    if (CloseGame) {
        return;
    }
    // Draw
    BeginDrawing();

    ClearBackground(BLACK);

    
    DrawTexture(gameBackground, 33, 33, GetColor(backgroundColour));
    DrawTexture(gameBorder, 0, 0, WHITE);

    if (!IsGamePaused && !IsEditing) {
        DrawText(("fps: " + std::to_string(timer->fps)).c_str(), 100, 5, 20, BLACK);
        DrawText("Pause: Q", GetScreenWidth() - MeasureText("Pause: Q", 20) - 100 , 5, 20, BLACK);
    }

    

    // Draw each object that has a sprite
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Draw();
    }

    if (!HasGameStarted && !IsGamePaused && !IsEditing) {
        float m = MeasureText("Press Space to begin.", 40);
        DrawText("Press Space to begin.", GetScreenWidth() / 2 - (m / 2), (GetScreenHeight() / 2) + 10, 42, BLACK);
        DrawText("Press Space to begin.", GetScreenWidth() / 2 - (m / 2), GetScreenHeight() / 2, 40, WHITE);

        Map map = Map();
        std::string mapName = map.GetMapName(currentMapIndex);
        float m2 = MeasureText(mapName.c_str(), 40);
        DrawText(mapName.c_str(), (GetScreenWidth() / 2) - (m2 / 2), (GetScreenHeight() / 2) + 10 + 76, 42, BLACK);
        DrawText(mapName.c_str(), (GetScreenWidth() / 2) - (m2 / 2), (GetScreenHeight() / 2) + 75, 40, WHITE);

    }

    if (player != nullptr && !IsGamePaused && !IsEditing) {
        for (int i = 0; i < player->lives; i++) {
            DrawTexture(paddleLife, WorldBorders.x + 16 + (80 * i), WorldBorders.w - 32, WHITE);
        }
    }
    


    EndDrawing();
}



Game::~Game() {

}