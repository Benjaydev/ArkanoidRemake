#include "Brick.h"
SpriteComponent* Brick::BrickReference;


Brick::Brick(float x, float y, int Health, int colour)
{
    tag = "Brick";


    physics->SetPosition(x, y);
    if (BrickReference == nullptr) {
        LoadSprite((char*)"Brick.png");
        BrickReference = sprite;
    }
    else {
        CopySpriteByReference(BrickReference);
    }
    

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
}

void Brick::DamageBrick()
{
    health--;

    std::cout << health << std::endl;

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
