#pragma once
#include "Timer.h"
#include "Object.h"
#include <iostream>
#include <vector>

class Game
{
public:


	Game();
	~Game();

	void Draw();

	void Update(float DeltaTime);

	Timer* timer = new Timer();
	float DeltaTime = 0;

	static std::vector<Object*> objects;

	static int lifetimeObjectCount;

	static int AddObjectToGame(Object* obj) {
		objects.push_back(obj);
		lifetimeObjectCount++;
		// Return count for object id
		return lifetimeObjectCount;
	}

};

