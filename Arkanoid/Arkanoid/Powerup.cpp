#include "Powerup.h"
#include "Game.h"
#include "Ball.h"

int Powerup::colours[3] = { 0x5555FFFF, 0xFF7700FF, 0xFF55CCFF };
Sound Powerup::PowerupSound = LoadSound((char*)"Powerup.wav");


Powerup::Powerup(float x, float y)
{
	tag = "Powerup";
	LoadSprite((char*)"Powerup.png");
	physics->SetPosition(x, y);

	physics->SetCollider(cType::Rectangle);
	Vector2 pos = { physics->globalTransform.m8 + sprite->GetWidth() / 2, physics->globalTransform.m9 + sprite->GetHeight() / 2 };
	physics->FitColliderWH(sprite->GetWidth(), sprite->GetHeight(), pos);

	physics->moveSpeed = 400;
	physics->maxSpeed = 400;
	physics->deceleration = 0;
	

	powerupType = rand() % 3 + 1;
	sprite->colour = GetColor(colours[powerupType-1]);

	AddToGameWorld();
}

Powerup::~Powerup()
{
}

void Powerup::CollideEvent(Hit hit, Object* other)
{
	if (startingCooldown > 0) {
		return;
	}
	if (hit.otherTag == "Wall") {
		// If the powerup hits bottom of screen
		if (hit.HitNormal.x == 0 && hit.HitNormal.y == -1) {
			isWaitingDestroy = true;
			hasBeenActivated = true;
			return;
		}
		return;
	}
	TriggerPowerup();
}

void Powerup::TriggerPowerup()
{
	if (hasBeenActivated) {
		return;
	}
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
			}
		}
	}
	// Increase player size
	else if (powerupType == 2) {
		Game::player->IncreasePlayerSize(20);
	}
	// Increase playerl lives
	else if (powerupType == 3) {
		Game::player->lives++;
	}



	hasBeenActivated = true;
	isWaitingDestroy = true;
}

void Powerup::Update(float DeltaTime)
{
	startingCooldown -= DeltaTime;
	physics->velocity = new Vector3({ 0, 100, 0 });
}
