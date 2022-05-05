#pragma once
#include "Timer.h"

class Game
{
public:

	Game();
	~Game();

	void Draw();

	void Update(float DeltaTime);

	Timer* timer = new Timer();
	float DeltaTime = 0;

};

