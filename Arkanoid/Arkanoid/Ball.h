#pragma once
#include "Object.h"
class Ball : public Object
{
public:
	Ball(float x, float y);
	virtual ~Ball();

	Object* ballSprite = new Object();
	Vector2 movementDirection = {1, -1};

	void Update(float DeltaTime) override;

	// Collisions
	void ReflectBall(Hit hit);
	void ReturnBall(Hit hit);
	void CollideEvent(Hit hit, Object* other) override;

	void CalculateDirection(float speed);

	static Sound BrickHitSound;
	static Sound PaddleHitSound;
};

