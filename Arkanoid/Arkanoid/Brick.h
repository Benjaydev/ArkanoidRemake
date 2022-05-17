#pragma once
#include "Object.h"
class Brick : public Object
{
public:
	Brick(float x, float y, int Health, int colour);
	virtual ~Brick();
	

	int maxHealth = 3;
	int health = 3;

	void DamageBrick();

	void CollideEvent(Hit hit, Object* other) override;


};

