#pragma once
#include "Ball.h"
#include "raymath.h"
#include "rlgl.h"
#include "Game.h"
using namespace std;

Object::Object()
{
}

Object::~Object()
{
	if (sprite != nullptr) {
		delete sprite;
	}
	

	// Unparent this object
	if(parent != nullptr){
		UnParent();
	}


	// Unparent all children from this object
	for (int i = 0; i < children.size(); i++) {
		children[i]->UnParent();
	}
	delete physics;

	RemoveFromGameWorld();
}

void Object::RemoveFromGameWorld() {
	Game::objects.erase(std::remove(Game::objects.begin(), Game::objects.end(), this), Game::objects.end());
}

void Object::AddToGameWorld() {
	// Remove if already added
	RemoveFromGameWorld();

	// Add to game
	id = Game::AddObjectToGame(this);

	// Add all children to game world
	for (int i = 0; i < children.size(); i++) {
		if (children[i]->id == 0) {
			children[i]->AddToGameWorld();
		}
		
	}
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
	

	// Flag parent to update physics children list
	parent->shouldReinstantiatePhysicsChildren = true;
	parent->UpdateChildPhysics();
}

void Object::UnParent() {
	
	// Contains child
	if (std::find(children.begin(), children.end(), this) != children.end()) {
		// Remove child from this object
		parent->children.erase(std::remove(parent->children.begin(), parent->children.end(), this), parent->children.end());
	}
	

	// Flag parent to update physics children list
	parent->shouldReinstantiatePhysicsChildren = true;
	parent->UpdateChildPhysics();

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

	UpdateChildPhysics();


}
void Object::UpdateChildPhysics() {
	// If the physics needs to update children (Called when the children in this object change)
	if (shouldReinstantiatePhysicsChildren) {
		// Reset children physics list
		physics->childrenPhysics.clear();
		// Re-add all current children physics
		for (int i = 0; i < children.size(); i++) {
			physics->childrenPhysics.push_back(children[i]->physics);
		}
		shouldReinstantiatePhysicsChildren = false;
	}
}


void Object::OnDraw()
{
}

void Object::Draw()
{
	OnDraw();

	if (hasSprite) {
		// Sprite Draw
		float rotation = (float)atan2(physics->globalTransform.m1, physics->globalTransform.m0);
		if (!WindowShouldClose()) {
			Vector2 position = { physics->globalTransform.m8, physics->globalTransform.m9 };

			DrawTextureEx(*sprite->texture, position, rotation * RAD2DEG, 1, sprite->colour);

		}
	}
	

	if (physics->collider != nullptr && Game::DebugActive) {
		physics->collider->DrawDebug();
	}
	
}
