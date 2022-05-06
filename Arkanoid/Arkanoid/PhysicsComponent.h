#pragma once
#include "PhysicsComponent.h"
#include <iostream>
#include "raylib.h"

class PhysicsComponent
{
public:
	PhysicsComponent();
	~PhysicsComponent();

	float maxSpeed = 200.0f;
	float moveSpeed = 100.0f;

	Vector3* velocity = new Vector3();
	Vector3* acceleration = new Vector3();
	float deceleration = 0.99f;

	Matrix* localTransform = new Matrix();
	Matrix* globalTransform = new Matrix();
	PhysicsComponent* parentPhysics;


	void Update(float DeltaTime);
	void UpdateTransform();
	void Translate(float x, float y);

	void Accelerate(float direction);
	void Decelerate();
	void Move(float DeltaTime);



	Vector3 Vector3FloatMultiply(Vector3 v1, float f);
	Vector3 Vector3FloatDivision(Vector3 v1, float f);
};

