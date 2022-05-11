#include "Brick.h"

Brick::Brick(float x, float y)
{
    tag = "Brick";


    physics->SetPosition(x, y);
    brickSprite->LoadSprite((char*)"Brick.png");

    AddChild(brickSprite);

    Vector2 brickPos = { physics->globalTransform->m8,physics->globalTransform->m9 };
    physics->SetCollider(cType::Rectangle);
    physics->FitColliderWH(brickSprite->sprite->GetWidth(), brickSprite->sprite->GetHeight(), brickPos);


    Vector2 offset = brickSprite->sprite->GetCentreOffset();
    brickSprite->physics->SetPosition(offset.x, offset.y);


    AddToGameWorld();



}

Brick::~Brick()
{
    delete brickSprite;
}
