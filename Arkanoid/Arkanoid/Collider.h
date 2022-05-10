#pragma once
#include "raylib.h"
#include "raymath.h"
#include <vector>
enum class cType { None, Rectangle, Circle, Ray };


class Collider
{
public:
	Collider();
	Collider(Collider* copy);
	~Collider();

	Collider* Copy(Collider* copy);

	cType type = cType::None;
	struct Hit {
		Vector3 OutVel = Vector3();
		Vector2 HitNormal = Vector2();
	};


	virtual bool Overlaps(Collider* other, Vector3 thisVel, Hit& result) { return false;  };
	virtual bool Overlaps(Vector2 point) { return false; };
	bool OverlapsScreen(Vector3 velocity, Hit& result);;

	virtual void DrawDebug() {};

	//virtual Vector3 CalculateNormal(Vector3 point) = 0;
	//virtual Vector3 CalculateNormal(Vector3 point) = 0;
	//virtual Vector3 CalculateNormal(Vector3 point) = 0;

	virtual bool IsEmpty() { return false; };

	virtual void Fit(Vector3 points[]) {};
	virtual void Fit(std::vector<Vector3> points) {};

	virtual void Inflate(Collider* other) {};

	virtual void Translate(float x, float y) {};

	virtual Vector2 ClosestPoint(Vector2 point) { return { 0,0 }; }

};


class RectangleCollider : public Collider
{
public:
	RectangleCollider();
	RectangleCollider(Vector3 otherMin, Vector3 otherMax);
	RectangleCollider(RectangleCollider* copy);
	~RectangleCollider();


	Vector3& min = boundries[0];
	Vector3& max = boundries[1];

	std::vector<Vector3> GetCorners() {
		std::vector<Vector3> corners;
		corners.push_back(min);
		corners.push_back({ min.x, max.y, min.z });
		corners.push_back(max);
		corners.push_back({ max.x, min.y, min.z });
		return corners;
	}

	Vector3 Center();
	Vector3 Extents() { return { abs(max.x - min.x) * 0.5f, abs(max.y - min.y) * 0.5f, abs(max.z - min.z) * 0.5f }; }

	void DrawDebug() override;


	bool IsEmpty();
	void Empty();

	void Fit(std::vector<Vector3> points);
	void Fit(Vector3 points[]);

	bool Overlaps(Vector2 point) override;
	bool Overlaps(Collider* other, Vector3 thisVel, Hit& result) override;

	void Inflate(Collider* other) override;

	Vector2 ClosestPoint(Vector2 point) override;

	void Translate(float x, float y) override;

protected:
	Vector3 boundries[2];

};

class CircleCollider : public Collider
{
public:
	CircleCollider();
	CircleCollider(Vector2 point, float r);
	CircleCollider(CircleCollider* copy);
	~CircleCollider();

	Vector2 center;
	float radius;

	void DrawDebug() override;


	bool IsEmpty();
	void Empty();

	void Fit(std::vector<Vector3> points);
	void Fit(Vector3 points[]);

	bool Overlaps(Vector2 point) override;
	bool Overlaps(Collider* other, Vector3 thisVel, Hit& result) override;

	void Inflate(Collider* other) override;

	void Translate(float x, float y) override;

	Vector2 ClosestPoint(Vector2 point) override;
};

class RayCollider : public Collider
{
public:
	RayCollider(Vector3 start, Vector3 dir, float len = FLT_MAX);
	~RayCollider();

	Vector3 origin = Vector3();
	Vector3 direction = Vector3();
	float length;


	Vector2 ClosestPoint(Vector2 point) override;

	bool Intersects(Collider* other, Vector3& Intersection);
};

