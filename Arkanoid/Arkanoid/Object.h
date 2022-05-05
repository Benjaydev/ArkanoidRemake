#pragma once
#include "PhysicsComponent.h"
#include <iostream>
#include <list>
#include "raylib.h"

class Object
{
public:

	Object();
	~Object();

	Object* parent = new Object();
	std::list<Object> children = {};

	bool isWaitingDestroy = false;


protected:
	Matrix* localTransform = new Matrix();
	Matrix* globalTransform = new Matrix();
	
	PhysicsComponent* physics = new PhysicsComponent();




};

