#pragma once
#include "Timer.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include "PauseMenu.h"
class Game
{
public:


	Game();
	~Game();

	void Draw();

	void Update(float DeltaTime);

	void StartGame(int mapIndex);
	void ResetGameObjects();


	Timer* timer = new Timer();
	float DeltaTime = 0;

	Texture2D background;

	Player* player;

	PauseMenu* pauseMenu;



	static std::vector<Object*> objects;

	static int lifetimeObjectCount;

	static int AddObjectToGame(Object* obj) {
		objects.push_back(obj);
		lifetimeObjectCount++;
		// Return count for object id
		return lifetimeObjectCount;
	}

	static bool DebugActive;

	static bool IsGamePaused;
	static void TogglePause() {
		IsGamePaused = !IsGamePaused;

		/*delete pauseMenu;
		pauseMenu = nullptr;
		if (IsGamePaused) {
			pauseMenu = new PauseMenu(GetScreenWidth() / 2, GetScreenHeight() / 2);
		}*/
	}



};

