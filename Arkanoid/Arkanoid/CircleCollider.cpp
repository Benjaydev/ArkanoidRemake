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
	DrawCircleLines(center.x, center.y, radius, RED);
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
	// Invalidate extents 
	Vector3 min = Vector3({ FLT_MAX, FLT_MAX, FLT_MAX });
	Vector3 max = Vector3({FLT_MIN, FLT_MIN, FLT_MIN});

	// Find min and max of the points 
	for (int i = 0; i < points.size(); ++i)
	{
		min = Vector3Min(min, points[i]);
		max = Vector3Max(max, points[i]);
	}

	// Put a circle around the min/max box 
	Vector3 add = Vector3Add(min, max);
	center = {add.x*0.5f, add.y*0.5f};
	radius = Vector3Length(Vector3Subtract(max, { center.x, center.y }));
}

void CircleCollider::Fit(Vector3 points[])
{
	// Invalidate extents 
	Vector3 min = Vector3({ FLT_MAX, FLT_MAX, FLT_MAX });
	Vector3 max = Vector3({ FLT_MIN, FLT_MIN, FLT_MIN });

	// Find min and max of the points 
	for (int i = 0; i < sizeof(points)/sizeof(Vector3); ++i)
	{
		min = Vector3Min(min, points[i]);
		max = Vector3Max(max, points[i]);
	}

	// Put a circle around the min/max box 
	Vector3 add = Vector3Add(min, max);
	center = { add.x * 0.5f, add.y * 0.5f };
	radius = Vector3Length(Vector3Subtract(max, { center.x, center.y }));
}

bool CircleCollider::Overlaps(Vector2 point)
{
	Vector2 p = Vector2Subtract(point, center);
	return Vector2DotProduct(p, p) <= (radius * radius);;
}

bool CircleCollider::Overlaps(Collider* other, Vector3* thisVel, Hit& result)
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

void CircleCollider::Inflate(Collider* other)
{

	if (other->type == cType::Circle) {
		CircleCollider* cir = (CircleCollider*)other;

		radius += cir->radius;
	}
}

void CircleCollider::Translate(float x, float y)
{

	center.x += x;
	center.y += y;
}

Vector2 CircleCollider::ClosestPoint(Vector2 point)
{
	return Vector2();
}

