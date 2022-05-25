#include "Brick.h"
#include "Game.h"
#include "Powerup.h"



Brick::Brick(float x, float y, int Health, int colour)
{
    tag = "Brick";

    // Keep track of bricks in game
    Game::BrickCount++;
    
    // Setup object
    physics->SetPosition(x, y);
    LoadSprite((char*)"Images/Brick.png");
    

    // Create collider for object based on sprite
    physics->SetCollider(cType::Rectangle);
    physics->FitColliderWH(sprite->GetWidth(), sprite->GetHeight(), { physics->globalTransform.m8 + sprite->GetWidth() / 2,physics->globalTransform.m9 + sprite->GetHeight() / 2 });

    // Add to game
    AddToGameWorld();

    maxHealth = Health;
    health = Health;

    sprite->colour = GetColor(colour); 

    // Lock axes so brick never moves
    physics->LockAxis = {1, 1};

}

Brick::~Brick()
{
    // Remove from game
    Game::BrickCount--;
}

void Brick::DamageBrick()
{
    // Remove 1 health
    health--;

    // If brick has no more health
    if (health <= 0) {

        // Destroy
        isWaitingDestroy = true;

        // Random chance to spawn powerup
        if (rand() % 15 + 1 == 1) {
           new Powerup(physics->globalTransform.m8, physics->globalTransform.m9);
         }

        return;
    }

    // Change opacity based on health
    sprite->colour.a = 255 * ((float)health / (float)maxHealth); 

}

void Brick::CollideEvent(Hit hit, Object* other)
{
    DamageBrick();
}
