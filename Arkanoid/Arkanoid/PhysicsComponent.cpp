#pragma once
#include "PhysicsComponent.h"
#include "raymath.h"
using namespace std;

PhysicsComponent::PhysicsComponent()
{
	*localTransform = MatrixIdentity();
	*globalTransform = MatrixIdentity();
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::Update(float DeltaTime)
{
	Move(DeltaTime);
}

void PhysicsComponent::UpdateTransform()
{
	// Calculate global transform if this object has parent transform
	if (parentPhysics != nullptr) {
		*globalTransform = MatrixMultiply(*parentPhysics->globalTransform, *localTransform);
	}
	// Default to using local transform
	else {
		globalTransform = localTransform;
	}
	
}


void PhysicsComponent::Accelerate(float direction)
{
	Vector3 facing;
	
	facing.x = localTransform->m0;
	facing.y = localTransform->m1;
	facing.z = 1;
	Vector3FloatMultiply(facing, moveSpeed * direction);

	*acceleration = Vector3Add(*acceleration, facing);
}

void PhysicsComponent::Decelerate()
{
	*velocity = Vector3FloatMultiply(*velocity, deceleration);
}

void PhysicsComponent::Move(float DeltaTime)
{
	*velocity = Vector3Add(*velocity, *acceleration);
	Decelerate();


	// Keep velocity within max speed
	float mag = Vector3Length(*velocity);
	if (mag != 0) {
		// Normalise direction
		Vector3 norm = Vector3FloatDivision(*velocity, mag);
		// Cap the speed at max speed for velocity
		float speed = fminf(mag, maxSpeed);
		// Set velocity to the speed
		*velocity = Vector3FloatMultiply(norm, speed);
	}
	acceleration = new Vector3();

	//std::cout << velocity->x << std::endl;
	Translate(velocity->x * DeltaTime, velocity->y * DeltaTime);
	UpdateTransform();
}


void PhysicsComponent::Translate(float x, float y) {
	localTransform->m8 += x;
	localTransform->m9 += y;
	
	*localTransform = MatrixMultiply(*localTransform, MatrixRotateZ(-0.005 * DEG2RAD));
}



Vector3 PhysicsComponent::Vector3FloatMultiply(Vector3 v1, float f){
	Vector3 v3;
	v3.x = v1.x * f;
	v3.y = v1.y * f;
	v3.z = v1.z * f;
	return v3;
}

Vector3 PhysicsComponent::Vector3FloatDivision(Vector3 v1, float f){
	Vector3 v3;
	v3.x = v1.x / f;
	v3.y = v1.y / f;
	v3.z = v1.z / f;
	return v3;
}
