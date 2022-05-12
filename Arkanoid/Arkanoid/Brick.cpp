#include "Brick.h"

Brick::Brick(float x, float y)
{
    tag = "Brick";


    physics->SetPosition(x, y);
    LoadSprite((char*)"Brick.png");

    Vector2 brickPos = { physics->globalTransform.m8 + sprite->GetWidth()/2,physics->globalTransform.m9 + sprite->GetHeight()/2 };
    physics->SetCollider(cType::Rectangle);
    physics->FitColliderWH(sprite->GetWidth(), sprite->GetHeight(), brickPos);

    AddToGameWorld();

    sprite->colour = GetColor(0x00FFFFFF);
    physics->LockAxis = {1, 1};

}

Brick::~Brick()
{
}

void Brick::DamageBrick()
{
    isWaitingDestroy = true;

}

void Brick::CollideEvent(Hit hit, Object* other)
{
    DamageBrick();
}
