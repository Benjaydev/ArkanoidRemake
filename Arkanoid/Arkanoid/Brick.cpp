#include "Brick.h"
#include "Game.h"


Brick::Brick(float x, float y, int Health, int colour)
{
    Game::BrickCount++;
    tag = "Brick";


    physics->SetPosition(x, y);
    LoadSprite((char*)"Brick.png");
    

    Vector2 brickPos = { physics->globalTransform.m8 + sprite->GetWidth()/2,physics->globalTransform.m9 + sprite->GetHeight()/2 };
    physics->SetCollider(cType::Rectangle);
    physics->FitColliderWH(sprite->GetWidth(), sprite->GetHeight(), brickPos);

    AddToGameWorld();

    maxHealth = Health;
    health = Health;

    

    sprite->colour = GetColor(colour); 
    physics->LockAxis = {1, 1};

}

Brick::~Brick()
{
    Game::BrickCount--;
}

void Brick::DamageBrick()
{
    health--;

    if (health <= 0) {
        isWaitingDestroy = true;
        return;
    }

    // Change opacity based on health
    sprite->colour.a = 255 * ((float)health / (float)maxHealth);
    
   

}

void Brick::CollideEvent(Hit hit, Object* other)
{
    DamageBrick();
}
