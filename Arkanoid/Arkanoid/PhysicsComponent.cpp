#pragma once
#include "PhysicsComponent.h"
#include "raymath.h"
#include "Game.h"
using namespace std;

PhysicsComponent::PhysicsComponent()
{
	*localTransform = MatrixIdentity();
	*globalTransform = MatrixIdentity();
}

PhysicsComponent::~PhysicsComponent()
{
	if (collider != nullptr) {
		delete collider;
	}
	delete globalTransform;
	delete localTransform;
	delete velocity;
	delete acceleration;

	
}

void PhysicsComponent::SetCollider(cType type)
{
	if (type == cType::Rectangle) {
		collider = new RectangleCollider();
	}
	else if (type == cType::Circle) {
		collider = new CircleCollider();
	}
}

void PhysicsComponent::FitColliderWH(float width, float height)
{
	if (collider != nullptr) {
		std::vector<Vector3> points = { {globalTransform->m8 - (width / 2), globalTransform->m9 - (height / 2), 0 }, {globalTransform->m8 + (width / 2), globalTransform->m9 + (height / 2), 0 } };
		collider->Fit(points);
	}
	
}

void PhysicsComponent::Update(float DeltaTime)
{
	CalculateVelocity(DeltaTime);
}

void PhysicsComponent::UpdateTransform()
{
	// Calculate global transform if this object has parent transform
	if (parentPhysics != nullptr) {
		*globalTransform = MatrixMultiply(*localTransform, *parentPhysics->globalTransform);
	}
	// Default to using local transform
	else {
		globalTransform = localTransform;
	}

	// Update transform for every child physics component
	for (int i = 0; i < childrenPhysics.size(); i++) {
		childrenPhysics[i]->UpdateTransform();
	}
	
}


void PhysicsComponent::Accelerate(float direction)
{
	Vector3 facing;
	
	facing.x = localTransform->m0;
	facing.y = localTransform->m1;
	facing.z = 1;
	facing = Vector3FloatMultiply(facing, moveSpeed * direction);

	*acceleration = Vector3Add(*acceleration, facing);
}

void PhysicsComponent::Decelerate()
{
	*velocity = Vector3FloatMultiply(*velocity, deceleration);
}

void PhysicsComponent::CalculateVelocity(float DeltaTime)
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
		// Adjust for delta time
		velocity->x *= DeltaTime;
		velocity->y *= DeltaTime;
	}
	acceleration = new Vector3();

}

void PhysicsComponent::Move()
{
	Translate(velocity->x, velocity->y);
}


void PhysicsComponent::Translate(float x, float y) {
	localTransform->m8 += x;
	localTransform->m9 += y;
	collider->Translate(x, y);

	

	UpdateTransform();
}

void PhysicsComponent::Translate(Vector2 v)
{
	Translate(v.x, v.y);
}


void PhysicsComponent::SetPosition(float x, float y)
{
	localTransform->m8 = x;
	localTransform->m9 = y;
	UpdateTransform();
}

void PhysicsComponent::SetPosition(Vector2 v)
{
	localTransform->m8 = v.x;
	localTransform->m9 = v.y;
	UpdateTransform();
}

void PhysicsComponent::SetRotation(float zRad)
{
	Matrix m = MatrixMultiply(MatrixIdentity(), MatrixRotateZ(zRad));
	*localTransform = m;
	UpdateTransform();
}

void PhysicsComponent::Rotate(float rad) {
	*localTransform = MatrixMultiply(MatrixRotateZ(rad), *localTransform);
	UpdateTransform();
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

void PhysicsComponent::GlobalCollisionCheck()
{
	for (int i = 0; i < Game::objects.size(); i++) {
		Object* object1 = Game::objects[i];
		for (int j = i + 1; j < Game::objects.size(); j++) {
			Object* object2 = Game::objects[j];
			// Don't check collision if object is child of other object
			if (object2->parent == object1 || object1->physics->collider == nullptr || object2->physics->collider == nullptr) {
				continue;
			}
			
			bool collided = object1->physics->collider->Overlaps(object2->physics->collider, object1->physics->velocity, object2->physics->velocity);
			if (!collided) {
				object1->physics->Move();
				object2->physics->Move();
			}
		}
	}

}
