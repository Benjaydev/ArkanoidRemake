#pragma once
#include "PhysicsComponent.h"
#include "raymath.h"
#include "Game.h"
using namespace std;

PhysicsComponent::PhysicsComponent()
{
	localTransform = MatrixIdentity();
	globalTransform = MatrixIdentity();
}

PhysicsComponent::~PhysicsComponent()
{
	if (collider != nullptr) {
		delete collider;
	}
	delete velocity;
	velocity = nullptr;
	delete acceleration;
	acceleration = nullptr;

	delete parentPhysics;

	childrenPhysics.clear();
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

void PhysicsComponent::FitColliderWH(float width, float height, Vector2 pos)
{
	if (collider != nullptr) {
		std::vector<Vector3> points = { {pos.x - (width / 2), pos.y - (height / 2), 0 }, {pos.x + (width / 2), pos.y + (height / 2), 0 } };
		collider->Fit(points);
	}
	
}

void PhysicsComponent::Update(float DeltaTime)
{
	CalculateVelocity(DeltaTime);
}

void PhysicsComponent::UpdateTransform()
{
	Vector2 lastpos = { globalTransform.m8, globalTransform.m9 };

	// Calculate global transform if this object has parent transform
	if (parentPhysics != nullptr) {
		globalTransform = MatrixMultiply(localTransform, parentPhysics->globalTransform);
	}
	// Default to using local transform
	else {
		globalTransform = localTransform;
	}

	// Update transform for every child physics component
	for (int i = 0; i < childrenPhysics.size(); i++) {
		childrenPhysics[i]->UpdateTransform();
	}


	// Update collider position
	if (collider != nullptr) {
		collider->Translate(globalTransform.m8- lastpos.x,  globalTransform.m9- lastpos.y);
	}
	
}


void PhysicsComponent::Accelerate(float direction)
{
	Vector3 facing;
	
	facing.x = localTransform.m0;
	facing.y = localTransform.m1;
	facing.z = 1;
	facing = Vector3FloatMultiply(facing, moveSpeed * direction);

	*acceleration = Vector3Add(*acceleration, facing);
}

void PhysicsComponent::Decelerate(float DeltaTime)
{
	*velocity = Vector3FloatMultiply(*velocity, 1-(deceleration * DeltaTime));
}

void PhysicsComponent::CalculateVelocity(float DeltaTime)
{
	*velocity = Vector3Add(*velocity, *acceleration);
	Decelerate(DeltaTime);


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
	
	// Reset the acceleration
	acceleration->x = 0;
	acceleration->y = 0;
	acceleration->z = 0;

}

void PhysicsComponent::Move(float DeltaTime)
{
	// If collider has a parent, apply the forces to the parent instead
	if (parentPhysics != nullptr) {
		if (parentPhysics->velocity != nullptr) {
			parentPhysics->velocity->x += velocity->x;
			parentPhysics->velocity->y += velocity->y;

			velocity->x = 0;
			velocity->y = 0;
			return;
		}
	}


	if (LockAxis.x && LockAxis.y) {
		return;
	}

	velocity->x = fminf(velocity->x, maxSpeed);
	velocity->y = fminf(velocity->y, maxSpeed);

	if (LockAxis.x && !LockAxis.y) {
		Translate(velocity->x * DeltaTime,0);
		return;
	}
	if (!LockAxis.x && LockAxis.y) {
		Translate(0, velocity->y * DeltaTime);
		return;
	}


	Translate(velocity->x * DeltaTime, velocity->y * DeltaTime);
}


void PhysicsComponent::Translate(float x, float y) {
	localTransform.m8 += x;
	localTransform.m9 += y;
	

	UpdateTransform();
}

void PhysicsComponent::Translate(Vector2 v)
{
	Translate(v.x, v.y);
}


void PhysicsComponent::SetPosition(float x, float y)
{
	float diffx = x - localTransform.m8;
	float diffy = y - localTransform.m9;
	Translate(diffx, diffy);
}

void PhysicsComponent::SetPosition(Vector2 v)
{
	localTransform.m8 = v.x;
	localTransform.m9 = v.y;
	UpdateTransform();
}

void PhysicsComponent::SetRotation(float zRad)
{
	Matrix m = MatrixMultiply(MatrixIdentity(), MatrixRotateZ(zRad));
	localTransform = m;
	UpdateTransform();
}

void PhysicsComponent::Rotate(float rad) {
	localTransform = MatrixMultiply(MatrixRotateZ(rad), localTransform);
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

void PhysicsComponent::GlobalCollisionCheck(float DeltaTime)
{
	if (DeltaTime == 0) {
		return;
	}
	for (int i = 0; i < Game::objects.size(); i++) {
		Object* check = Game::objects[i];
		if (check->physics->collider == nullptr || !check->physics->hasPhysicsCheck) {
			check->physics->Move(DeltaTime);
			continue;
		}
		
		Hit wallResult;
		if (check->physics->collider->OverlapsScreen(check->physics->deltaVelocity(DeltaTime), wallResult)) {
			check->physics->velocity->x = wallResult.OutVel.x / DeltaTime;
			check->physics->velocity->y = wallResult.OutVel.y / DeltaTime;

			if (check->tag == "Ball") {
				wallResult.otherTag = "Wall";
				check->CollideEvent(wallResult, nullptr);
			}
		}

		for (int j = i+1; j < Game::objects.size(); j++) {
			Object* against = Game::objects[j];

			// Don't check collision if object does not have physics checks, is child of the other object, if object is null, or if both objects have no velocity
			if (!against->physics->hasPhysicsCheck && against->parent == check || against->physics->collider == nullptr || ((check->physics->velocity->x == 0.0f && check->physics->velocity->y == 0.0f) && (against->physics->velocity->x == 0.0f && against->physics->velocity->y == 0.0f))) {
				continue;
			}

			// Do base collision check
			Hit result;


			
			//*check->physics->velocity = check->physics->deltaVelocity(DeltaTime);
			bool collided = check->physics->collider->Overlaps(against->physics->collider, check->physics->deltaVelocity(DeltaTime), against->physics->deltaVelocity(DeltaTime), result);

			


			if (collided) {
				if (check->tag == "Ball" && (against->tag == "Player" || against->tag == "LeftPlayerEnd" || against->tag == "RightPlayerEnd" || against->tag == "Brick")) {
					result.otherTag = against->tag;

					check->physics->velocity->x = result.OutVel.x / DeltaTime;
					check->physics->velocity->y = result.OutVel.y / DeltaTime;
					check->physics->Move(DeltaTime);

					check->CollideEvent(result, against);

				}
				if (against->tag == "Ball" && (check->tag == "Player" || check->tag == "LeftPlayerEnd" || check->tag == "RightPlayerEnd" || check->tag == "Brick")) {
					result.otherTag = check->tag;

					against->physics->velocity->x = result.OutVel.x / DeltaTime;
					against->physics->velocity->y = result.OutVel.y / DeltaTime;
					against->physics->Move(DeltaTime);

					against->CollideEvent(result, check);
				}
			
				/*
				if (check->tag == "Ball" && against->tag == "Ball") {
					result.otherTag = "Ball";
					check->CollideEvent(result, against);

					Hit newHit = Hit({ result.OutVel, Vector2Negate(result.HitNormal), result.percentDistanceAlongHitFace, result.otherTag });
					against->CollideEvent(newHit, check);
				}*/

			}
			
			// Set colliding if has collided atleast once
			check->physics->isColliding = check->physics->isColliding ? true : collided;
			against->physics->isColliding = against->physics->isColliding ? true : collided;
		}
		check->physics->Move(DeltaTime);
		
	}

}
