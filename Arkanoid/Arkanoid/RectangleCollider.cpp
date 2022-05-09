#include "Collider.h"

RectangleCollider::RectangleCollider() {
	type = cType::Rectangle;
}

RectangleCollider::RectangleCollider(Vector3 otherMin, Vector3 otherMax)
{
	min = otherMin;
	max = otherMax;
	type = cType::Rectangle;
}

RectangleCollider::RectangleCollider(RectangleCollider* copy)
{
	min = Vector3(copy->min);
	max = Vector3(copy->max);
	type = cType::Rectangle;
}

RectangleCollider::~RectangleCollider() {
	Empty();
}

void RectangleCollider::Translate(float x, float y)
{
	min.x += x;
	max.x += x;
	min.y += y;
	max.y += y;
}

Vector3 RectangleCollider::Center()
{
	Vector3 center = Vector3Add(min, max);
	center.x *= 0.5f;
	center.y *= 0.5f;
	center.z *= 0.5f;
	return center;

}

void RectangleCollider::DrawDebug()
{
	std::vector<Vector3> c = GetCorners();
	DrawLine((int)c[0].x, (int)c[0].y, (int)c[1].x, (int)c[1].y, RED);
	DrawLine((int)c[1].x, (int)c[1].y, (int)c[2].x, (int)c[2].y, RED);
	DrawLine((int)c[2].x, (int)c[2].y, (int)c[3].x, (int)c[3].y, RED);
	DrawLine((int)c[3].x, (int)c[3].y, (int)c[0].x, (int)c[0].y, RED);

	Vector3 center = Center();
	DrawCircle((int)c[0].x, (int)c[0].y, 3, VIOLET);
	DrawCircle((int)c[3].x, (int)c[3].y, 3, GREEN);
	DrawCircle((int)c[1].x, (int)c[1].y, 3, BLUE);
	DrawCircle((int)c[2].x, (int)c[2].y, 3, ORANGE);
}

bool RectangleCollider::IsEmpty()
{
	return (isinf(min.x) && isinf(min.y) && isinf(min.z) && isinf(max.x) && isinf(max.y) && isinf(max.z));
}

void RectangleCollider::Empty()
{
	boundries[0] = { -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };
	boundries[1] = { std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
}

void RectangleCollider::Fit(std::vector<Vector3> points)
{
	Empty();

	min = points[0];
	max = points[0];

	for (int i = 0; i < points.size(); i++) {
		min = Vector3Min(min, points[i]);
		max = Vector3Max(max, points[i]);
	}
}

void RectangleCollider::Fit(Vector3 points[])
{
	Empty();

	min = points[0];
	max = points[0];

	for (int i = 0; i < sizeof(points)/sizeof(Vector3); i++) {
		min = Vector3Min(min, points[i]);
		max = Vector3Max(max, points[i]);
	}
}

bool RectangleCollider::Overlaps(Vector2 point)
{
	return !(point.x < min.x || point.y < min.y || point.x > max.x || point.y > max.y);
}


bool RectangleCollider::Overlaps(Collider* other, Vector3* thisVel, Vector3* otherVel)
{
	if (other->type == cType::Rectangle) {
		RectangleCollider* rec = (RectangleCollider*)other;
		return !(max.x + thisVel->x < rec->min.x + otherVel->x || max.y + thisVel->y < rec->min.y + otherVel->y || min.x + thisVel->x > rec->max.x + otherVel->x || min.y + thisVel->y > rec->max.y + otherVel->y);
	}
	else if (other->type == cType::Circle) {
		CircleCollider* cir = (CircleCollider*)other;
		cir->Overlaps(this, thisVel, otherVel);
	}

}
	

Vector2 RectangleCollider::ClosestPoint(Vector2 point)
{
	float x = Clamp(point.x, min.x, max.x);
	float y = Clamp(point.y, min.y, max.y);
	return { x, y };
}
