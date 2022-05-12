#include "Player.h"

Player::Player(float x, float y)
{
    tag = "Player";

    physics->SetPosition(x, y);

    float scale = 4;
    centerSegment->LoadSprite((char*)"CenterSegment.png");
    centerSegment->sprite->SetScale(scale);
    
    //centerSegmentRight->LoadSprite((char*)"CenterSegment.png");
    //centerSegmentRight->sprite->SetScale(scale);
   //centerSegmentRight->sprite->texture->width /= 2;

    leftEnd->LoadSprite((char*)"LeftEnd.png");
    leftEnd->sprite->SetScale(scale);

    rightEnd->LoadSprite((char*)"RightEnd.png");
    rightEnd->sprite->SetScale(scale);


    Vector2 playerPos = { physics->globalTransform.m8,physics->globalTransform.m9 };


    physics->SetCollider(cType::Rectangle);
    physics->FitColliderWH(centerSegment->sprite->GetWidth() + leftEnd->sprite->GetWidth() + rightEnd->sprite->GetWidth(), centerSegment->sprite->GetHeight(), playerPos);

  
    
    AddChild(centerSegment);
    //AddChild(centerSegmentRight);
    AddChild(leftEnd);
    AddChild(rightEnd);

    Vector2 offset = centerSegment->sprite->GetCentreOffset();

    centerSegment->physics->SetPosition({ offset.x, offset.y});
    leftEnd->physics->SetPosition({ -(centerSegment->sprite->GetWidth()), offset.y });
    rightEnd->physics->SetPosition({ rightEnd->sprite->GetWidth(), offset.y });


    AddToGameWorld();

    physics->moveSpeed = 100;
    physics->maxSpeed = 600;
    physics->deceleration = 5;
}

Player::~Player()
{
    centerSegment->isWaitingDestroy = true;
    leftEnd->isWaitingDestroy = true;
    rightEnd->isWaitingDestroy = true;
}

void Player::Update(float DeltaTime)
{
   

    Object::Update(DeltaTime);

}
