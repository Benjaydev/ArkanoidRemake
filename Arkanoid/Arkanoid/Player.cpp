#include "Player.h"

Player::Player(float x, float y)
{
    tag = "Player";

    // Setup object position
    physics->SetPosition(x, y);

    float scale = 4;

    // Create player paddle (Is split into 3 parts so that the center can be expanded at a later point without stretching textures)
    // Center segment
    centerSegment->LoadSprite((char*)"Images/CenterSegment.png");
    centerSegment->sprite->SetScale(scale);
    
    // Left end sprite
    leftEnd->LoadSprite((char*)"Images/LeftEnd.png");
    leftEnd->sprite->SetScale(scale);
    leftEnd->tag = "LeftPlayerEnd";


    // Right end sprite
    rightEnd->LoadSprite((char*)"Images/RightEnd.png");
    rightEnd->sprite->SetScale(scale);
    rightEnd->tag = "RightPlayerEnd";


    // Add all sprites to this object
    AddChild(centerSegment);
    AddChild(leftEnd);
    AddChild(rightEnd);

    // Calculate colliders and sprites
    CalculateSpritesAndColliders();

    // Add to game world
    AddToGameWorld();


    // Setup movement values
    physics->moveSpeed = 10;
    physics->maxSpeed = 800;
    physics->deceleration = 7.5;
}

Player::~Player()
{
    // Destroy sprites
    centerSegment->isWaitingDestroy = true;
    leftEnd->isWaitingDestroy = true;
    rightEnd->isWaitingDestroy = true;
}

void Player::Update(float DeltaTime)
{
    Object::Update(DeltaTime);

    // Decrease size until back to default
    if (size > 20) {
        sizeCooldown -= DeltaTime;
        if (sizeCooldown <= 0) {
          IncreasePlayerSize(-20);
        }
    }
    

}


void Player::IncreasePlayerSize(float amount)
{
    // Keep track of size
    size += amount;
    // Reset cooldown
    sizeCooldown = 15;

    // Expand the center sprite
    centerSegment->sprite->texture->width += amount;

    // Re-calculate sprites and colliders
    CalculateSpritesAndColliders();
}

void Player::CalculateSpritesAndColliders()
{
    // Set collider for center
    physics->SetCollider(cType::Rectangle);
    physics->FitColliderWH(centerSegment->sprite->GetWidth() + centerSegment->sprite->GetWidth() / 8, centerSegment->sprite->GetHeight(), { physics->globalTransform.m8,physics->globalTransform.m9 });


    // Get sprite offsets
    Vector2 offset = centerSegment->sprite->GetCentreOffset();
    Vector2 leftOffset = leftEnd->sprite->GetCentreOffset();
    Vector2 rightOffset = rightEnd->sprite->GetCentreOffset();

    // Calculate correct relative positions
    centerSegment->physics->SetPosition({ offset.x, offset.y });
    leftEnd->physics->SetPosition({ offset.x - leftEnd->sprite->GetWidth(), leftOffset.y });
    rightEnd->physics->SetPosition({ offset.x + centerSegment->sprite->GetWidth(), rightOffset.y });

    // Add colliders for both ends of paddle
    leftEnd->physics->SetCollider(cType::Circle);
    leftEnd->physics->FitColliderWH(leftEnd->sprite->GetWidth(), 0, { leftEnd->physics->globalTransform.m8 + leftEnd->sprite->GetWidth() / 1.5f ,leftEnd->physics->globalTransform.m9 + leftEnd->sprite->GetHeight() / 2 });
   
    rightEnd->physics->SetCollider(cType::Circle);
    rightEnd->physics->FitColliderWH(rightEnd->sprite->GetWidth(), 0, { rightEnd->physics->globalTransform.m8 + rightEnd->sprite->GetWidth() / 2.5f ,rightEnd->physics->globalTransform.m9 + rightEnd->sprite->GetHeight() / 2 });


}

