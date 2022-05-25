#include "Powerup.h"
#include "Game.h"
#include "Ball.h"

int Powerup::colours[3] = { 0x5555FFFF, 0xFF7700FF, 0xFF55CCFF };
Sound Powerup::PowerupSound = LoadSound((char*)"Sounds/Powerup.wav");


Powerup::Powerup(float x, float y)
{
	tag = "Powerup";

	// Setup object
	LoadSprite((char*)"Images/Powerup.png");
	physics->SetPosition(x, y);

	// Create collider
	physics->SetCollider(cType::Rectangle);
	physics->FitColliderWH(sprite->GetWidth(), sprite->GetHeight(), { physics->globalTransform.m8 + sprite->GetWidth() / 2, physics->globalTransform.m9 + sprite->GetHeight() / 2 });


	// Setup movement values
	physics->moveSpeed = 400;
	physics->maxSpeed = 400;
	physics->deceleration = 0;
	
	// Randomise powerup type
	powerupType = rand() % 3 + 1;
	sprite->colour = GetColor(colours[powerupType-1]);


	// Add to game world
	AddToGameWorld();
}


void Powerup::CollideEvent(Hit hit, Object* other)
{
	// Do not collider at start of life
	if (startingCooldown > 0) {
		return;
	}

	// Check if powerup has reached bottom
	if (hit.otherTag == "Wall") {
		// If the powerup hits bottom of screen
		if (hit.HitNormal.x == 0 && hit.HitNormal.y == -1) {
			// Destroy
			isWaitingDestroy = true;
			hasBeenActivated = true;
			return;
		}
		return;
	}
	// Else trigger powerup
	TriggerPowerup();
}

void Powerup::TriggerPowerup()
{
	// Only trigger powerup once
	if (hasBeenActivated) {
		return;
	}

	// Play sound
	PlaySound(PowerupSound);

	// Ball duplication powerup
	if (powerupType == 1) {
		// Search for all balls in game
		int size = Game::objects.size();
		for (int i = 0; i < size; i++) {
			if (Game::objects[i]->tag == "Ball") {
				// Cast the object into ball
				Ball* b = (Ball*)Game::objects[i];

				// Create first ball that travels in opposite x direction
				Ball* ball = new Ball(b->physics->globalTransform.m8, b->physics->globalTransform.m9);
				ball->movementDirection = {-(b->movementDirection.x), (b->movementDirection.y) };
				
				// Create second ball that travels in opposite y direction
				Ball* ball2 = new Ball(b->physics->globalTransform.m8, b->physics->globalTransform.m9);
				ball2->movementDirection = {(b->movementDirection.x), -(b->movementDirection.y) };


				ball = nullptr;
				ball2 = nullptr;
				b = nullptr;
			}
		}
	}
	// Increase player size
	else if (powerupType == 2) {
		Game::player->IncreasePlayerSize(20);
	}
	// Increase player lives
	else if (powerupType == 3) {
		Game::player->lives++;
	}


	// Destroy this powerup
	hasBeenActivated = true;
	isWaitingDestroy = true;
}

void Powerup::Update(float DeltaTime)
{
	startingCooldown -= DeltaTime;

	// Keep velocity going downwards
	physics->velocity->y = 100;
	physics->velocity->x = 0;
}
