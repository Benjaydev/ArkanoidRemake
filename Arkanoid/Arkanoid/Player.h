#pragma once
#include "Object.h"
class Player : public Object
{
public:
	Player(float x, float y);
	virtual ~Player();

	// Sprite pieces
	Object* centerSegment = new Object();
	Object* rightEnd = new Object();
	Object* leftEnd = new Object();

	int lives = 3;

	float sizeCooldown = 15;
	float size = 0;


	void Update(float DeltaTime) override;

	void CalculateSpritesAndColliders();

	void IncreasePlayerSize(float amount);
};

