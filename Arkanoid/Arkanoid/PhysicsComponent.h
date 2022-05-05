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




};

