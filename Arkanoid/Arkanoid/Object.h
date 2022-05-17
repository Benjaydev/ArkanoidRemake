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
	virtual ~Object();

	// Flags
	bool shouldReinstantiatePhysicsChildren = false;

	int id = 0;
	std::string tag = "";
	bool hasSprite = false;
	bool usesReferencedSprite = false;
	SpriteComponent* sprite = nullptr;

	Object* parent = nullptr;
	std::vector<Object*> children;

	bool isWaitingDestroy = false;

	void AddToGameWorld();
	void RemoveFromGameWorld();
	void DeleteSelf();


	void DeleteChild(Object* child);
	void AddChild(Object* child);
	void RemoveChild(Object* child);

	void ParentTo(Object* p);
	void UnParent();


	void UpdateChildPhysics();


	virtual void OnUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);

	virtual void CollideEvent(Hit hit, Object* other){};

	virtual void OnDraw();
	virtual void Draw();

	void LoadSprite(char* filename);
	void CopySpriteByReference(SpriteComponent* s);


	PhysicsComponent* physics = new PhysicsComponent();


	bool operator== (const Object* other) const { return id == other->id; }
	bool operator!= (const Object* other) const { return !operator==(other); }

protected:





};

