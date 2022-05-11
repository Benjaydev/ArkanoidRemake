#pragma once
#include "Object.h"
class Ball : public Object
{
public:
	Ball(float x, float y);
	~Ball();

	Object* ballSprite = new Object();
	Vector2 movementDirection = {1, 1};


	void Update(float DeltaTime) override;


	void ReflectBall(Hit hit);

	void ReturnBall(Hit hit);

	void CollideEvent(Hit hit) override;

	void CalculateDirection(float speed);
};

