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
		DeleteChild(this);
	}


	// Unparent all children frokm this object
	for (Object *child : children) {
		child->parent = nullptr;
	}

	RemoveFromGameWorld();
}

void Object::RemoveFromGameWorld() {
	Game::objects.erase(std::remove(Game::objects.begin(), Game::objects.end(), this), Game::objects.end());
}

void Object::AddToGameWorld() {
	Game::objects.push_back(this);
}

void Object::DeleteChild(Object* child) {
	// Remove child from this object
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
	// Remove parent from child
	child->parent = nullptr;
	// Remove from game
	child->RemoveFromGameWorld();

	delete &child;
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
}

void Object::OnDraw()
{
}

void Object::Draw()
{
	OnDraw();

	float rotation = (float)atan2(globalTransform->m1, globalTransform->m0);

	if (!WindowShouldClose()) {
		Vector2 position = { globalTransform->m8, globalTransform->m9 };
		// Draw sprite to screen
		DrawTextureEx(*sprite->texture, position, rotation * RAD2DEG, sprite->textureScale, sprite->colour);
	}

}
