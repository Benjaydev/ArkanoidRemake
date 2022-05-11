#pragma once
#include "Object.h"
class Brick : public Object
{
public:
	Brick(float x, float y);
	~Brick();

	Object* brickSprite = new Object();

};

