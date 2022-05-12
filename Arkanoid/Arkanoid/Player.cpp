#include "Player.h"

Player::Player(float x, float y)
{
    tag = "Player";

    physics->SetPosition(x, y);

    float scale = 4;
    centerSegment->LoadSprite((char*)"CenterSegment.png");
    centerSegment->sprite->SetScale(scale);
    

    leftEnd->LoadSprite((char*)"LeftEnd.png");
    leftEnd->sprite->SetScale(scale);
    leftEnd->tag = "LeftPlayerEnd";

    rightEnd->LoadSprite((char*)"RightEnd.png");
    rightEnd->sprite->SetScale(scale);
    rightEnd->tag = "RightPlayerEnd";



    AddChild(centerSegment);
    AddChild(leftEnd);
    AddChild(rightEnd);

    
    CalculateSpritesAndColliders();


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


void Player::IncreasePlayerSize(float amount)
{
    centerSegment->sprite->texture->width += amount;
    CalculateSpritesAndColliders();

}

void Player::CalculateSpritesAndColliders()
{
    Vector2 playerPos = { physics->globalTransform.m8,physics->globalTransform.m9 };
    physics->SetCollider(cType::Rectangle);
    physics->FitColliderWH(centerSegment->sprite->GetWidth() + centerSegment->sprite->GetWidth() / 8, centerSegment->sprite->GetHeight(), playerPos);


    Vector2 offset = centerSegment->sprite->GetCentreOffset();

    Vector2 leftOffset = leftEnd->sprite->GetCentreOffset();
    Vector2 rightOffset = rightEnd->sprite->GetCentreOffset();

    centerSegment->physics->SetPosition({ offset.x, offset.y });
    leftEnd->physics->SetPosition({ offset.x - leftEnd->sprite->GetWidth(), leftOffset.y });
    rightEnd->physics->SetPosition({ offset.x + centerSegment->sprite->GetWidth(), rightOffset.y });


    leftEnd->physics->SetCollider(cType::Circle);
    leftEnd->physics->FitColliderWH(leftEnd->sprite->GetWidth(), 0, { leftEnd->physics->globalTransform.m8 + leftEnd->sprite->GetWidth() / 2 ,leftEnd->physics->globalTransform.m9 + leftEnd->sprite->GetHeight() / 2 });


    rightEnd->physics->SetCollider(cType::Circle);
    rightEnd->physics->FitColliderWH(rightEnd->sprite->GetWidth(), 0, { rightEnd->physics->globalTransform.m8 + rightEnd->sprite->GetWidth() / 2 ,rightEnd->physics->globalTransform.m9 + rightEnd->sprite->GetHeight() / 2 });


}

