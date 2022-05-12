#pragma once
#include "Object.h"
class Brick : public Object
{
public:
	Brick(float x, float y);
	virtual ~Brick();
	


	void DamageBrick();

	void CollideEvent(Hit hit, Object* other) override;
};

