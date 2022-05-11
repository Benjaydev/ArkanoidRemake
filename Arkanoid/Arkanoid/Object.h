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

	// Flags
	bool shouldReinstantiatePhysicsChildren = false;

	int id;
	std::string tag = "";
	bool hasSprite = false;
	SpriteComponent* sprite;


	Object* parent;
	std::vector<Object*> children;

	bool isWaitingDestroy = false;

	void AddToGameWorld();
	void RemoveFromGameWorld();
	void DeleteChild(Object* child);
	void AddChild(Object* child);
	void RemoveChild(Object* child);

	void ParentTo(Object* p);
	void UnParent();


	virtual void OnUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);

	virtual void CollideEvent(Hit hit){};

	virtual void OnDraw();
	virtual void Draw();

	void LoadSprite(char* filename);


	PhysicsComponent* physics = new PhysicsComponent();


	bool operator== (const Object* other) const { return id == other->id; }
	bool operator!= (const Object* other) const { return !operator==(other); }

protected:





};

