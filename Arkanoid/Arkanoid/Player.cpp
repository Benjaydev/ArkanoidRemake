#include "Player.h"

Player::Player(float x, float y)
{
    physics->SetPosition(x, y);

    float scale = 4;
    centerSegment->LoadSprite((char*)"CenterSegment.png");
    centerSegment->sprite->SetScale(scale);

    leftEnd->LoadSprite((char*)"LeftEnd.png");
    leftEnd->sprite->SetScale(scale);

    rightEnd->LoadSprite((char*)"RightEnd.png");
    rightEnd->sprite->SetScale(scale);




    physics->SetCollider(cType::Rectangle);
    physics->FitColliderWH(centerSegment->sprite->GetWidth() + leftEnd->sprite->GetWidth() + rightEnd->sprite->GetWidth(), centerSegment->sprite->GetHeight());

    AddChild(centerSegment);
    AddChild(leftEnd);
    AddChild(rightEnd);

    Vector2 pos = centerSegment->sprite->GetCentreOffset();
    centerSegment->physics->SetPosition(pos);
    leftEnd->physics->SetPosition({ pos.x + pos.x, pos.y});
    rightEnd->physics->SetPosition({ pos.x - pos.x*2, pos.y });




    AddToGameWorld();

    physics->moveSpeed = 100;
    physics->maxSpeed = 600;
    physics->deceleration = 0.99;
}

Player::~Player()
{
}

void Player::Update(float DeltaTime)
{
   

    Object::Update(DeltaTime);

}
