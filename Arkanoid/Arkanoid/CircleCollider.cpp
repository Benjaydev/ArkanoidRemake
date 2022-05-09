#include "Collider.h"

CircleCollider::CircleCollider()
{
	type = cType::Circle;
}
CircleCollider::CircleCollider(Vector2 point, float r)
{
	center = point;
	radius = r;
	type = cType::Circle;
}
CircleCollider::CircleCollider(CircleCollider* copy)
{
	center = Vector2(copy->center);
	radius = copy->radius;
	type = cType::Circle;
}
CircleCollider::~CircleCollider()
{


}

void CircleCollider::DrawDebug()
{
}

bool CircleCollider::IsEmpty()
{
	return false;
}

void CircleCollider::Empty()
{
}

void CircleCollider::Fit(std::vector<Vector3> points)
{
}

void CircleCollider::Fit(Vector3 points[])
{
}

bool CircleCollider::Overlaps(Vector2 point)
{
	Vector2 p = Vector2Subtract(point, center);
	return Vector2DotProduct(p, p) <= (radius * radius);;
}

bool CircleCollider::Overlaps(Collider* other, Vector3* thisVel, Vector3* otherVel)
{
	if (other->type == cType::Rectangle) {
		RectangleCollider* rec = (RectangleCollider*)other;
		Vector2 closest = rec->ClosestPoint(center);
		return Overlaps(closest);
	}
	else if (other->type == cType::Circle) {
		CircleCollider* cir = (CircleCollider*)other;
		Vector2 diff = Vector2Subtract(cir->center, center);

		float r = radius + cir->radius;
		return Vector2DotProduct(diff, diff) <= (r * r);
	}
	
}

void CircleCollider::Translate(float x, float y)
{

	center.x += x;
	center.y += y;
}

