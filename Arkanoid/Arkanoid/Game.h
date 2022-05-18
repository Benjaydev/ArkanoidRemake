#pragma once
#include "Timer.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include "PauseMenu.h"
#include "MainMenu.h"
#include "LevelEditor.h"
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
	void ResetGameObjects();

	void TogglePauseMenu();



	Timer* timer = new Timer();
	float DeltaTime = 0;

	Texture2D gameBackground;
	int backgroundColour = 0x333333FF;
	
	Player* player;

	PauseMenu* pauseMenu;
	MainMenu* mainMenu;
	LevelEditor* levelEditor;

	std::vector<std::function<void()>> storeDestroy;
	void DestroyStoredAwaiting();



	static std::vector<Object*> objects;

	static int lifetimeObjectCount;

	static int AddObjectToGame(Object* obj) {
		objects.push_back(obj);
		lifetimeObjectCount++;
		// Return count for object id
		return lifetimeObjectCount;
	}

	static void TogglePause() { IsGamePaused = !IsGamePaused; }

	static bool DebugActive;

	static bool IsGamePaused;




};

