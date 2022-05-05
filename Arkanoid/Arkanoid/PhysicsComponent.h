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

	Vector3 velocity;
	Vector3 acceleration;
	float deceleration = 0.99f;


};

