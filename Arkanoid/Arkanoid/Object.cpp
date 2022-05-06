#pragma once
#include "Object.h"
#include "Game.h"
#include "raymath.h"
using namespace std;

Object::Object()
{
}

Object::~Object()
{
	// Unparent this object
	if(parent != nullptr){
		DeleteChild(this);
	}


	// Unparent all children frokm this object
	for (Object *child : children) {
		child->UnParent();
	}

	RemoveFromGameWorld();
}

void Object::RemoveFromGameWorld() {
	Game::objects.erase(std::remove(Game::objects.begin(), Game::objects.end(), this), Game::objects.end());
}

void Object::AddToGameWorld() {
	Game::objects.push_back(this);
}

void Object::AddChild(Object* child) {
	child->ParentTo(this);
	
}
void Object::RemoveChild(Object* child) {
	child->UnParent();
}

void Object::DeleteChild(Object* child) {
	
	// Remove parent from child
	child->UnParent();
	// Remove from game
	child->RemoveFromGameWorld();

	delete &child;
}


void Object::ParentTo(Object* p) {
	// Set parent
	parent = p;
	// Parent physics
	physics->parentPhysics = parent->physics;

	// Add to children vector
	parent->children.push_back(this);
}

void Object::UnParent() {
	
	// Remove child from this object
	parent->children.erase(std::remove(children.begin(), children.end(), this), children.end());
	// Unparent
	parent = nullptr;
	// Unparent physics
	physics->parentPhysics = nullptr;
}



void Object::LoadSprite(char* filename) {
	sprite = new SpriteComponent();
	sprite->Load(filename);
	hasSprite = true;
}


void Object::OnUpdate(float DeltaTime)
{
}

void Object::Update(float DeltaTime)
{
	OnUpdate(DeltaTime);
	physics->Update(DeltaTime);
}



void Object::OnDraw()
{
}

void Object::Draw()
{
	OnDraw();

	float rotation = (float)atan2(physics->globalTransform->m1, physics->globalTransform->m0);

	if (!WindowShouldClose()) {
		Vector2 position = { physics->globalTransform->m8, physics->globalTransform->m9 };
		// Draw sprite to screen
		DrawTextureEx(*sprite->texture, position, rotation * RAD2DEG, sprite->textureScale, sprite->colour);
	}

}
