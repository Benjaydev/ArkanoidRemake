#include "Ball.h"
#include "Hit.h"
#include "Game.h"

Sound Ball::BrickHitSound = LoadSound((char*)"BrickHit.wav");
Sound Ball::PaddleHitSound = LoadSound((char*)"PaddleHit.wav");

Ball::Ball(float x, float y)
{
    tag = "Ball";
    Game::ActiveBalls++;

    physics->SetPosition(x, y);
    ballSprite->LoadSprite((char*)"Ball2.png");
    ballSprite->sprite->SetScale(1.1);
    

    AddChild(ballSprite);

    Vector2 ballPos = { physics->globalTransform.m8,physics->globalTransform.m9 };
    physics->SetCollider(cType::Circle);
    physics->FitColliderWH(ballSprite->sprite->GetWidth()/2, ballSprite->sprite->GetHeight()/2, ballPos);


    Vector2 offset = ballSprite->sprite->GetCentreOffset();

    ballSprite->physics->SetPosition(offset.x, offset.y);
    AddToGameWorld();


    physics->moveSpeed = 800;
    physics->maxSpeed = 800;
    physics->deceleration = 0.01f;

}

Ball::~Ball()
{
    Game::ActiveBalls--;
    ballSprite->isWaitingDestroy = true;
}

void Ball::Update(float DeltaTime)
{
    Object::Update(DeltaTime);

    CalculateDirection(physics->maxSpeed);
}
void Ball::CalculateDirection(float speed)
{
    if (movementDirection.y < 0.1f && movementDirection.y > -0.1f) {
        movementDirection.y *= 100;
    }
    Vector2 normDir = Vector2Normalize(movementDirection);
    physics->velocity->x = normDir.x * speed;
    
    physics->velocity->y = normDir.y * speed;

}

void Ball::ReflectBall(Hit hit)
{
    Vector2 refDir = Vector2(movementDirection);
    refDir.x *= hit.HitNormal.x;
    refDir.y *= hit.HitNormal.y;
    
    refDir.x *= 2;
    refDir.y *= 2;

    refDir.x *= hit.HitNormal.x;
    refDir.y *= hit.HitNormal.y;


    movementDirection = Vector2Subtract(movementDirection, refDir);

}



void Ball::ReturnBall(Hit hit)
{
    movementDirection = Vector2Negate(movementDirection);
}


void Ball::CollideEvent(Hit hit, Object* other)
{
    if (hit.otherTag == "Brick") {
        PlaySound(BrickHitSound);
        ReflectBall(hit);
        CalculateDirection(physics->maxSpeed);

        other->CollideEvent(hit, this);
        return;
    }
    if (hit.otherTag == "Wall") {
        // If the ball hits bottom of screen
        if (hit.HitNormal.x == 0 && hit.HitNormal.y == -1) {
            isWaitingDestroy = true;
            return;
        }
        ReflectBall(hit);
        CalculateDirection(physics->maxSpeed);
        return;
    }
    if (hit.otherTag == "Ball") {
        ReflectBall(hit);
        CalculateDirection(physics->maxSpeed);

        
        return;
    }


    PlaySound(PaddleHitSound);

    float facePos = hit.percentDistanceAlongHitFace;
    float speedMultiplier = 1;

    // If ball is moving left
    if (movementDirection.x < 0) {
        // If ball lands on right side of paddle
        if ((facePos >= 0.5 && hit.otherTag == "Player") || hit.otherTag == "RightPlayerEnd") {
            if (hit.otherTag == "Player") {
                // Convert facePos range from 0-0.5 to 0.5-1 to use for speed multiplier
                speedMultiplier = ((facePos - 0.5f) / (1.0f - 0.5f)) * (1.2f - 0.8f) + 0.8f;
                movementDirection.x = -1 * speedMultiplier;
            }
            
            ReturnBall(hit);
        }
        // If ball lands on left side of paddle
        else {
            if (hit.otherTag == "Player") {
                // Convert facePos range from 0-0.5 to 0.5-1 to use for speed multiplier
                speedMultiplier = ((facePos - 0) / (0.5f - 0)) * (1.2f - 0.8f) + 0.8f;
                movementDirection.x = -1 * speedMultiplier;
            }
            
            ReflectBall(hit);
        }
        CalculateDirection(physics->maxSpeed*speedMultiplier);
        return;
    }
    // If ball is moving right
    else if (movementDirection.x > 0) {
        // If ball lands on left side of paddle
        if ((facePos <= 0.5 && hit.otherTag == "Player") || hit.otherTag == "LeftPlayerEnd") {
            if (hit.otherTag == "Player") {
                // Convert facePos range from 0-0.5 to 0.5-1 to use for speed multiplier
                speedMultiplier = ((facePos - 0) / (0.5f - 0)) * (1.2f - 0.8f) + 0.8f;
                movementDirection.x = 1 * speedMultiplier;
            }
            ReturnBall(hit);
        }
        // If ball lands on right side of paddle
        else {
            if (hit.otherTag == "Player") {
                // Convert facePos range from 0-0.5 to 0.5-1 to use for speed multiplier
                speedMultiplier = ((facePos - 0.5f) / (1.0f - 0.5f)) * (1.2f - 0.8f) + 0.8f;


                movementDirection.x = 1 * speedMultiplier;
            }
            ReflectBall(hit);
        }
        
        CalculateDirection(physics->maxSpeed * speedMultiplier);
        return;
    }


    ReflectBall(hit);
    CalculateDirection(physics->maxSpeed);
}


