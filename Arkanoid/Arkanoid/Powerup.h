#pragma once
#include "Object.h"
class Powerup : public Object
{
public:
	Powerup(float x, float y);

	int powerupType = 1;
	bool hasBeenActivated = false;
	float startingCooldown = 2;

	void CollideEvent(Hit hit, Object* other);
	void TriggerPowerup();

	void Update(float DeltaTime);

	static int colours[3];

	static Sound PowerupSound;

};

