#pragma once
#include "Object.h"
#include "Game.h"

Object::Object()
{
}

Object::~Object()
{
	// Unparent this object
	if(parent != nullptr){
		DeleteChild(*this);
	}


	// Unparent all children frokm this object
	for (Object child : children) {
		child.parent = nullptr;
	}

	Game::objects.remove(*this);
}


void Object::AddToGameWorld() {
	for (Object child : children) {
		child.isWaitingDestroy = true;
	}
	Game::objects.push_back(*this);
}

void Object::DeleteChild(Object child) {
	// Remove child from this object
	children.remove(child);
	// Remove parent from child
	child.parent = nullptr;
	// Remove from game
	Game::objects.remove(child);

	delete &child;
}



void Object::OnUpdate(float DeltaTime)
{
}

void Object::Update(float DeltaTime)
{
	OnUpdate(DeltaTime);
}

void Object::OnDraw()
{
}

void Object::Draw()
{
	OnDraw();
}
