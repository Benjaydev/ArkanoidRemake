#pragma once
#include "Object.h"
class Player : public Object
{
public:
	Player(float x, float y);
	virtual ~Player();


	Object* centerSegment = new Object();
	Object* rightEnd = new Object();
	Object* leftEnd = new Object();


	void Update(float DeltaTime) override;
};

