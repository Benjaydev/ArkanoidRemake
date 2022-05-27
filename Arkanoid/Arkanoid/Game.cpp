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

// Init static values
Player* Game::player;
std::vector<Object*> Game::objects = std::vector<Object*>();
Game* Game::ThisGame = nullptr;
Vector4 Game::WorldBorders = { 38, 33, 886, 883 };
// Ints
int Game::lifetimeObjectCount = 0;
int Game::ActiveBalls = 0;
int Game::BrickCount = 0;
// Booleans
bool Game::DebugActive = false;
bool Game::IsGamePaused = false;
bool Game::IsEditing = false;
bool Game::HasGameStarted = false;
bool Game::IsStoryMode = false;
bool Game::CloseGame = false;


Game::Game() {
    // Setup window
    int screenWidth = 924;
    int screenHeight = 883;
    InitWindow(screenWidth, screenHeight, "Arkanoid - Ben Wharton");
   
    // Setup audio
    InitAudioDevice();

    // Store this game
    ThisGame = this;

    // Toggle console logs
    SetTraceLogLevel(LOG_NONE);
    if (DebugActive) {
        SetTraceLogLevel(LOG_ALL);
    }

    
    // Load background images
    gameBackground = LoadTexture("Images/Background.png");
    gameBorder = LoadTexture("Images/Border.png");
    // Load display paddle used for lives
    paddleLife = LoadTexture("Images/Paddle.png");


    // Load main menu
    StartMainMenu();

    // Main game loop
    // Detect window close button or ESC key
    while (!WindowShouldClose() && !CloseGame)    
    {
        // Get new time
        DeltaTime = timer->RecordNewTime();
        
        Update(DeltaTime);
        Draw();
    }

    delete timer;
    
    // Unload textures from memory
    UnloadTexture(gameBorder);
    UnloadTexture(gameBackground);
    UnloadTexture(paddleLife);

    // Unload sounds from memory
    UnloadSound(Powerup::PowerupSound);
    UnloadSound(Ball::BrickHitSound);
    UnloadSound(Ball::PaddleHitSound);

    // Remove all game objects
    ResetGameObjects();
    
    // Close audio
    CloseAudioDevice();

    // De-Initialization 
    CloseWindow();
}


void Game::ResetGameObjects() {
    // Remove menus //
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
    //_____________//

    // Store all objects for deletion
    for (int i = 0; i < objects.size(); i++) {
        storeDestroy.push_back(std::bind(&Object::DeleteSelf, objects[i]));
    }

    // Destroy objects
    DestroyStoredAwaiting(); 
}
void Game::DestroyStoredAwaiting()
{
    // Destroy all objects
    for (int del = 0; del < storeDestroy.size(); del++) {
        storeDestroy[del]();
    }
    storeDestroy.clear();
    storeDestroy.shrink_to_fit();
    objects.shrink_to_fit();
}



void Game::StartMainMenu()
{
    // Map saves are taken from Saves.txt
    Map::saveLocation = "Saves.txt";
    // Reset game objects
    ResetGameObjects();

    // Set flags
    IsStoryMode = false;
    IsGamePaused = true;
    IsEditing = false;

    // Set background colour
    backgroundColour = 0x333333FF;

    // Add main menu
    mainMenu = new MainMenu();
    mainMenu->levelEditButton->AssignCallMethod(std::bind(&Game::StartLevelEditor, this));
    mainMenu->storyModeButton->AssignCallMethod(std::bind(&Game::StartGameStory, this));
}

void Game::StartLevelEditor()
{
    // Reset game objects
    ResetGameObjects();

    // Set flags
    IsGamePaused = true;
    IsEditing = true;

    // Add level editor
    levelEditor = new LevelEditor();
    levelEditor->mainMenuButton->AssignCallMethod(std::bind(&Game::StartMainMenu, this));

}


void Game::StartGame(int index) {
    // Set the current map being played
    currentMapIndex = index;

    // Reset game objects
    ResetGameObjects();

    // Set flags
    IsGamePaused = false;
    IsEditing = false;
    HasGameStarted = false;
    
    // Load map
    Map map = Map();
    map.LoadMap(index);
    map.GenerateMap();
    backgroundColour = map.mapStruct.backgroundColour;

    // Create player
    player = new Player(GetScreenWidth() / 2.0f, (float) (GetScreenHeight() - 100));
    player->IncreasePlayerSize(20);

    // Create starting ball
    new Ball(player->physics->globalTransform.m8, player->physics->globalTransform.m9 - 30);


}

void Game::StartGameStory()
{
    IsStoryMode = true;
    // Change where saves are loaded from to StorySaves.txt
    Map::saveLocation = "StorySaves.txt";
    // Start the first level
    StartGame(0);
}


void Game::TogglePauseMenu() {
    IsGamePaused = !IsGamePaused;

    // Remove pause menu
    if (!IsGamePaused) {
        delete pauseMenu;
        pauseMenu = nullptr;
        return;
    }

    // Create pause menu
    pauseMenu = new PauseMenu(GetScreenWidth() / 2, GetScreenHeight() / 2);

    // Set the function calls for buttons
    pauseMenu->resumeButton->AssignCallMethod(std::bind(&Game::TogglePauseMenu, this));
    pauseMenu->mainmenuButton->AssignCallMethod(std::bind(&Game::StartMainMenu, this));
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

        // Store objects for later destroy
        if (objects[i]->isWaitingDestroy) {
            storeDestroy.push_back(std::bind(&Object::DeleteSelf, objects[i]));
            continue;
        }
    }
    // Destroy waiting objects
    DestroyStoredAwaiting();
    
    // If in level editor
    if (IsEditing) {
        // Set background colour to leveleditor map
        backgroundColour = levelEditor->map.mapStruct.backgroundColour;
        return;
    }

    // If game is active
    if (!IsGamePaused) {
        // If game has started
        if (HasGameStarted) {
            // Check collisions
            PhysicsComponent::GlobalCollisionCheck(DeltaTime);

            // Move player left
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
            {
                player->physics->Accelerate(-1);
            }
            // Move player right
            if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            {
                player->physics->Accelerate(1);
            }

            // Game win if there are no more bricks
            if (BrickCount == 0) {
                // If it is story mode, move on to the next map
                if (IsStoryMode) {
                    Map map = Map();
                    // If at the last level, return to main menu
                    if (currentMapIndex + 1 >= map.GetMapCount()) {
                        StartMainMenu();
                        return;
                    }
                    // Move to next level
                    StartGame(currentMapIndex + 1);
                }
                // If not in story mode, just return to main menu
                else {
                    StartMainMenu();
                    return;
                }
            }
            // Player death if there are no more balls
            if (ActiveBalls == 0) {

                // Remove player life
                player->lives--;

                // Game over case, return to main menu
                if (player->lives == -1) {
                    StartMainMenu();
                    return;
                }

                // If player has lives left
                HasGameStarted = false;
                // Reset position
                player->physics->SetPosition(GetScreenWidth() / 2.0f, (float) (GetScreenHeight() - 100));
                // Spawn new ball
                new Ball(player->physics->globalTransform.m8, player->physics->globalTransform.m9 - 30);
            }
        }
        
        // Start level
        if (IsKeyDown(KEY_SPACE)) {
            if (!HasGameStarted) {
                HasGameStarted = true;
            }
        }
    }

    // If not in main menu
    if (mainMenu == nullptr) {
        // Q to pause the level
        if (IsKeyPressed(KEY_Q)) {
            TogglePauseMenu();
        }
    }

}
void Game::Draw()
{
    // Return if game is meant to close
    if (CloseGame) {
        return;
    }

    BeginDrawing();

    // Reset screen
    ClearBackground(BLACK);
    // Draw background
    DrawTexture(gameBackground, 33, 33, GetColor(backgroundColour));
    DrawTexture(gameBorder, 0, 0, WHITE);

    // IF game is not pause or in edit mode
    if (!IsGamePaused && !IsEditing) {
        // Display frames per second
        DrawText(("fps: " + std::to_string(timer->fps)).c_str(), 100, 5, 20, BLACK);
        // Display pause control
        DrawText("Pause: Q", GetScreenWidth() - MeasureText("Pause: Q", 20) - 100 , 5, 20, BLACK);
    }

    

    // Draw each object
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->Draw();
    }

    // If lvel hasn't startes, game is not paused, and is not editing. Draw this text over top
    if (!HasGameStarted && !IsGamePaused && !IsEditing) {
        // Create text with shadow
        int m = MeasureText("Press Space to begin.", 40);
        DrawText("Press Space to begin.", GetScreenWidth() / 2 - (m / 2), (GetScreenHeight() / 2) + 10, 42, BLACK);
        DrawText("Press Space to begin.", GetScreenWidth() / 2 - (m / 2), GetScreenHeight() / 2, 40, WHITE);

        // Create text to display map name
        Map map = Map();
        std::string mapName = map.GetMapName(currentMapIndex);
        int m2 = MeasureText(mapName.c_str(), 40);
        DrawText(mapName.c_str(), (GetScreenWidth() / 2) - (m2 / 2), (GetScreenHeight() / 2) + 10 + 76, 42, BLACK);
        DrawText(mapName.c_str(), (GetScreenWidth() / 2) - (m2 / 2), (GetScreenHeight() / 2) + 75, 40, WHITE);

    }

    // If player is active, the game is not paused, and not in editing mode
    if (player != nullptr && !IsGamePaused && !IsEditing) {
        // Display the player lives
        for (int i = 0; i < player->lives; i++) {
            DrawTexture(paddleLife, (int) (WorldBorders.x + 16 + (80 * i)), (int)(WorldBorders.w - 32), WHITE);
        }
    }
    
    EndDrawing();
}


// Destructor
Game::~Game() {

}