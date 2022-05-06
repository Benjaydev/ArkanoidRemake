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

};

