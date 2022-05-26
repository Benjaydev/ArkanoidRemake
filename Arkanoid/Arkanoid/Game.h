#pragma once

#include "Timer.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include "PauseMenu.h"
#include "LevelEditor.h"
#include "MainMenu.h"
class Game
{
public:

	Game();
	~Game();

	Timer* timer = new Timer();
	float DeltaTime = 0;

	int currentMapIndex;

	Texture2D gameBackground;
	Texture2D gameBorder;
	Texture2D paddleLife;
	int backgroundColour = 0x333333FF;
	

	// Menus
	PauseMenu* pauseMenu;
	MainMenu* mainMenu;
	LevelEditor* levelEditor;

	std::vector<std::function<void()>> storeDestroy;
	void DestroyStoredAwaiting();


	void Draw();

	void Update(float DeltaTime);

	void StartMainMenu();
	void StartLevelEditor();
	void StartGame(int mapIndex);
	void StartGameStory();
	void TogglePauseMenu();

	void ResetGameObjects();


	// STATICS
	static Player* player;
	static Game* ThisGame;
	static Vector4 WorldBorders;

	static int lifetimeObjectCount;
	static int ActiveBalls;
	static int BrickCount;

	static std::vector<Object*> objects;
	static int AddObjectToGame(Object* obj) {
		objects.push_back(obj);
		lifetimeObjectCount++;
		// Return count for object id
		return lifetimeObjectCount;
	}


	static void TogglePause() { IsGamePaused = !IsGamePaused; }
	static void CloseGameWindow() { CloseGame = true; }

	// Flags
	static bool DebugActive;
	static bool IsGamePaused;
	static bool IsEditing;
	static bool HasGameStarted;
	static bool IsStoryMode;
	static bool CloseGame;



};