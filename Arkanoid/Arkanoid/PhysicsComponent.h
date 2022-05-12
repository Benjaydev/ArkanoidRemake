#pragma once
#include "PhysicsComponent.h"
#include "SpriteComponent.h"
#include <iostream>
#include <vector>
#include "raylib.h"
#include "Collider.h"

class PhysicsComponent
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	float maxSpeed = 200.0f;
	float moveSpeed = 100.0f;

	Vector3* velocity = new Vector3();
	Vector3 deltaVelocity(float DeltaTime) {
		return { velocity->x * DeltaTime, velocity->y * DeltaTime, velocity->z * DeltaTime };
	}
	Vector3* acceleration = new Vector3();
	float deceleration = 0.01f;

	Matrix localTransform = Matrix();
	Matrix globalTransform = Matrix();
	PhysicsComponent* parentPhysics = nullptr;
	std::vector<PhysicsComponent*> childrenPhysics;

	Vector2 LockAxis = { 0, 0 };

	Collider* collider = nullptr;

	bool isColliding = false;

	void SetCollider(cType type);
	void FitColliderWH(float width, float height, Vector2 pos);


	void Update(float DeltaTime);
	void UpdateTransform();
	void Translate(float x, float y);
	void Translate(Vector2 v);

	void Accelerate(float direction);
	void Decelerate(float DeltaTime);
	void CalculateVelocity(float DeltaTime);
	void Move(float DeltaTime);

	void SetPosition(float x, float y);
	void SetPosition(Vector2 v);
	void SetRotation(float zRad);
	void Rotate(float rad);


	static Vector3 Vector3FloatMultiply(Vector3 v1, float f);
	static Vector3 Vector3FloatDivision(Vector3 v1, float f);


	static void GlobalCollisionCheck(float DeltaTime);


};

