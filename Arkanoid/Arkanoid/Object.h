#pragma once
#include "PhysicsComponent.h"
#include <iostream>
#include <vector>
#include "raylib.h"
#include "SpriteComponent.h"

class Object
{
public:

	Object();
	~Object();

	int id;
	bool hasSprite = false;
	SpriteComponent* sprite = new SpriteComponent;


	Object* parent;
	std::vector<Object*> children;

	bool isWaitingDestroy = false;

	void AddToGameWorld();
	void RemoveFromGameWorld();
	void DeleteChild(Object* child);

	virtual void OnUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);

	virtual void OnDraw();
	virtual void Draw();

	void LoadSprite(char* filename);



	PhysicsComponent* physics = new PhysicsComponent();


	bool operator== (const Object* other) const { return id == other->id; }
	bool operator!= (const Object* other) const { return !operator==(other); }

protected:
	Matrix* localTransform = new Matrix();
	Matrix* globalTransform = new Matrix();





};

