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
	// Update collider position
	if (collider != nullptr) {
		collider->Translate(x, y);
	}
	

	UpdateTransform();
}

void PhysicsComponent::Translate(Vector2 v)
{
	Translate(v.x, v.y);
}


void PhysicsComponent::SetPosition(float x, float y)
{
	float diffx = x - localTransform->m8;
	float diffy = y - localTransform->m9;
	Translate(diffx, diffy);
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
		Object* check = Game::objects[i];
		for (int j = i + 1; j < Game::objects.size(); j++) {
			Object* against = Game::objects[j];
			// Don't check collision if object is child of other object
			if (against->parent == check || check->physics->collider == nullptr || against->physics->collider == nullptr || ((check->physics->velocity->x==0.0f && check->physics->velocity->y == 0.0f) && (against->physics->velocity->x == 0.0f && against->physics->velocity->y == 0.0f)) ) {
				continue;
			}

			// Do base collision check
			Collider::Hit result;
			bool collided = check->physics->collider->Overlaps(against->physics->collider, check->physics->velocity, result);

			if (collided) {
				check->physics->velocity->x = result.OutVel.x;
				check->physics->velocity->y = result.OutVel.y;

				against->physics->velocity->x = -result.OutVel.x;
				against->physics->velocity->y = -result.OutVel.y;
				
				
			}
			
			/*
			if (!collided) {
				// If no collision is found, use a ray check.
				//// This check will allow collision to be detected even if the objects next frame position has passed through
				// to the other side of the oter object. 
				// In the rare case that the object is travelling fast enough or each frame is slow enough for the object to move through another object,
				// a raycast can be used to check for any collision in between this frame and the next frame

				// First, an "inflated" version of the other object collider needs to be made
				// Create a copy of the other objeccts collider
				Collider* inflatedCol = new Collider();
				inflatedCol = inflatedCol->Copy(against->physics->collider);
				// This basically just adds the size of our check object collider to the against object collider
				inflatedCol->Inflate(check->physics->collider);
				inflatedCol->DrawDebug(); // TEMPORARY DEBUG

				// Get the future position of the check object
				Vector3 ForwardPosition = { check->physics->globalTransform->m8 + check->physics->velocity->x, check->physics->globalTransform->m9 + check->physics->velocity->y, 0 };
				// Current position
				Vector3 CurrentPosition = { check->physics->globalTransform->m8, check->physics->globalTransform->m9, 0 };

				// Get the direction between the current position and the future position
				Vector3 dir = Vector3Subtract(ForwardPosition, CurrentPosition);
				// Get the distance for how far the ray should travel
				float dist = Vector3Length(dir);

				// Create new ray that goes from the current position to the future position in order to check for collisions inbetween
				RayCollider ray = RayCollider(CurrentPosition, Vector3Normalize(dir), dist);

				// Check for collision against the inflated collider
				Vector3 result = Vector3();
				bool rayCollided = ray.Intersects(inflatedCol, result);
				
				collided = rayCollided;

				// If this ray has intersected, return object to the hit location
				if (rayCollided) {
					check->physics->SetPosition(result.x, result.y);

				}

				delete inflatedCol;
			}*/

			

			// Set colliding if has collided atleast once
			check->physics->isColliding = check->physics->isColliding ? true : collided;
			against->physics->isColliding = against->physics->isColliding ? true : collided;
		}

		check->physics->Move();
		
	}

}
