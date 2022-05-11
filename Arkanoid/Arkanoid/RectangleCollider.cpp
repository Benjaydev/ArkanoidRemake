#include "Collider.h"
#include <string>
using namespace std;

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
	RectangleCollider* rec = (RectangleCollider*)copy;
	min = Vector3(rec->min);
	max = Vector3(rec->max);
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

	std::string str = std::to_string(min.x) + ", " + std::to_string(min.y);
	DrawText(str.c_str(), (int)c[0].x-20, (int)c[0].y-20, 12, BLUE);

	DrawLine((int)c[0].x, (int)c[0].y, (int)c[1].x, (int)c[1].y, RED);
	DrawLine((int)c[1].x, (int)c[1].y, (int)c[2].x, (int)c[2].y, RED);
	DrawLine((int)c[2].x, (int)c[2].y, (int)c[3].x, (int)c[3].y, RED);
	DrawLine((int)c[3].x, (int)c[3].y, (int)c[0].x, (int)c[0].y, RED);

	
	DrawCircle((int)c[0].x, (int)c[0].y, 3, VIOLET);
	DrawCircle((int)c[3].x, (int)c[3].y, 3, GREEN);
	DrawCircle((int)c[1].x, (int)c[1].y, 3, BLUE);
	DrawCircle((int)c[2].x, (int)c[2].y, 3, ORANGE);

	Vector3 center = Center();
	DrawCircle(center.x, center.y, 4, RED);
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


bool RectangleCollider::Overlaps(Collider* other, Vector3 thisVel, Vector3 otherVel, Hit& result)
{
	if (other->type == cType::Rectangle) {
		RectangleCollider* rec = (RectangleCollider*)other;

		// Check if objects are already colliding
		if (!(max.x < rec->min.x || max.y < rec->min.y || min.x > rec->max.x || min.y > rec->max.y)) {
			return true;
		}

		Vector3 relV = Vector3Negate(thisVel);

		float hitTime = 0.0f;
		float outTime = 1.0f;
		Vector2 overlapTime = Vector2Zero();

		// X axis overlap
		if (relV.x < 0)
		{
			if (rec->max.x < min.x) { return false; }
			if (rec->max.x > min.x) { 
				outTime = fminf((min.x - rec->max.x) / relV.x, outTime);
			}

			if (max.x < rec->min.x)
			{
				overlapTime.x = (max.x - rec->min.x) / relV.x;
				hitTime = fmaxf(overlapTime.x, hitTime);
			}
		}
		else if (relV.x > 0)
		{
			if (rec->min.x > max.x) { return false; }
			if (max.x > rec->min.x) {
				outTime = fminf((max.x - rec->min.x) / relV.x, outTime);
			}

			if (rec->max.x < min.x)
			{
				overlapTime.x = (min.x - rec->max.x) / relV.x;
				hitTime = fmaxf(overlapTime.x, hitTime);
			}
		}

		if (hitTime > outTime) {
			return false;
		}



		// Y axis overlap
		if (relV.y < 0)
		{
			if (rec->max.y < min.y) { return false; }
			if (rec->max.y > min.y) {
				outTime = fminf((min.y - rec->max.y) / relV.y, outTime);
			}

			if (max.y < rec->min.y)
			{
				overlapTime.y = (max.y - rec->min.y) / relV.y;
				hitTime = fmaxf(overlapTime.y, hitTime);
			}
		}
		else if (relV.y > 0)
		{
			if (rec->min.y > max.y) { return false; }
			if (max.y > rec->min.y) {
				outTime = fminf((max.y - rec->min.y) / relV.y, outTime);
			}

			if (rec->max.y < min.y)
			{
				overlapTime.y = (min.y - rec->max.y) / relV.y;
				hitTime = fmaxf(overlapTime.y, hitTime);
			}
		}

		if (hitTime > outTime) {
			return false;
		}


		// Scale resulting velocity by normalized hit time
		result.OutVel = Vector3(relV);
		result.OutVel.x *= -hitTime;
		result.OutVel.y *= -hitTime;

		result.HitNormal = Vector2();
		// Hit normal is along axis with the highest overlap time
		if (overlapTime.x > overlapTime.y)
		{
			result.HitNormal = { relV.x >= 0 ? 1.0f : -1.0f, 0 };
		}
		else
		{
			result.HitNormal = { 0, relV.y >= 0 ? 1.0f : -1.0f };
		}
		return true;

	}
	else if (other->type == cType::Circle) {
		CircleCollider* cir = (CircleCollider*)other;
		cir->Overlaps(this, otherVel, thisVel, result);
	}

}

void RectangleCollider::Inflate(Collider* other)
{
	if (other->type == cType::Rectangle) {
		RectangleCollider* rec = (RectangleCollider*)other;
		Vector3 extents = rec->Extents();

		min.x -= extents.x;
		min.y -= extents.y;

		max.x += extents.x;
		max.y += extents.y;
	}
	if (other->type == cType::Circle) {
		CircleCollider* cir = (CircleCollider*)other;

		min.x -= cir->radius;
		min.y -= cir->radius;
		max.x += cir->radius;
		max.y += cir->radius;
	}

}
	

Vector2 RectangleCollider::ClosestPoint(Vector2 point)
{
	float x = Clamp(point.x, min.x, max.x);
	float y = Clamp(point.y, min.y, max.y);
	return { x, y };
}
