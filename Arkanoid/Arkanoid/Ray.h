#pragma once
#include "Collider.h"
class RayCollider : public Collider
{
public:
	RayCollider(Vector3 start, Vector3 direction, float length = FLT_MAX);
	~RayCollider();

	Vector3 origin;
	Vector3 direction;
	float length;


	Vector3 ClosestPoint(Vector3* point);

	bool Overlaps(Vector2 point) override = 0;
	bool Overlaps(Collider* other, Vector3* thisVel, Vector3* otherVel) override = 0;

	bool Intersects(Collider* other, Vector3& Intersection);
};

