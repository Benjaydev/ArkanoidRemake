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

	void Draw();

	void Update(float DeltaTime);

	void StartMainMenu();
	void StartLevelEditor();
	void StartGame(int mapIndex);
	void StartGameChoice(int mapIndex);
	void StartGameStory();


	void ResetGameObjects();

	void TogglePauseMenu();



	Timer* timer = new Timer();
	float DeltaTime = 0;

	int currentMapIndex;


	Texture2D gameBackground;
	Texture2D gameBorder;
	Texture2D paddleLife;
	int backgroundColour = 0x333333FF;
	
	



	static Player* player;

	PauseMenu* pauseMenu;
	MainMenu* mainMenu;
	LevelEditor* levelEditor;

	std::vector<std::function<void()>> storeDestroy;
	void DestroyStoredAwaiting();


	static Game* ThisGame;

	static Vector4 WorldBorders;

	static std::vector<Object*> objects;

	static int lifetimeObjectCount;

	static int AddObjectToGame(Object* obj) {
		objects.push_back(obj);
		lifetimeObjectCount++;
		// Return count for object id
		return lifetimeObjectCount;
	}

	static int ActiveBalls;
	static int BrickCount;


	static void TogglePause() { IsGamePaused = !IsGamePaused; }

	static void CloseGameWindow() { 
		CloseGame = true;
		CloseWindow();

	}

	static bool DebugActive;
	static bool IsGamePaused;
	static bool IsEditing;
	static bool HasGameStarted;
	static bool IsStoryMode;
	static bool CloseGame;



};