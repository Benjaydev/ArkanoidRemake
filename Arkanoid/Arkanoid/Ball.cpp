#include "Ball.h"
#include "Hit.h"
#include "Game.h"

Sound Ball::BrickHitSound = LoadSound((char*)"Sounds/BrickHit.wav");
Sound Ball::PaddleHitSound = LoadSound((char*)"Sounds/PaddleHit.wav");

Ball::Ball(float x, float y)
{
    tag = "Ball";

    // Keep track of ball count in game
    Game::ActiveBalls++;

    // Setup sprite object
    physics->SetPosition(x, y);
    ballSprite->LoadSprite((char*)"Images/Ball2.png");
    ballSprite->sprite->SetScale(1.1f);
    
    // Add sprite
    AddChild(ballSprite);

    // Create collider fit to the sprite
    physics->SetCollider(cType::Circle);
    physics->FitColliderWH(ballSprite->sprite->GetWidth()/2.0f, ballSprite->sprite->GetHeight()/ 2.0f, { physics->globalTransform.m8,physics->globalTransform.m9 });


    // Offset sprite to center of object
    Vector2 offset = ballSprite->sprite->GetCentreOffset();
    ballSprite->physics->SetPosition(offset.x, offset.y);
    AddToGameWorld();

    // Set movement values
    physics->moveSpeed = 800;
    physics->maxSpeed = 800;
    physics->deceleration = 0.01f;

}

Ball::~Ball()
{
    // Remove from game
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
    // Calculate velocity based on the balls intended movement direction
    Vector2 normDir = Vector2Normalize(movementDirection);
    physics->velocity->x = normDir.x * speed;
    physics->velocity->y = normDir.y * speed;

}

void Ball::ReflectBall(Hit hit)
{
    // Use reflection equation to calculate reflected direction
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
    // Reverse the direction
    movementDirection = Vector2Negate(movementDirection);
}


void Ball::CollideEvent(Hit hit, Object* other)
{
    // Ball hits brick
    if (hit.otherTag == "Brick") {
        PlaySound(BrickHitSound);

        // Reflect and calculate velocity
        ReflectBall(hit);
        CalculateDirection(physics->maxSpeed);

        other->CollideEvent(hit, this);
        return;
    }
    // Ball hits wall
    if (hit.otherTag == "Wall") {
        // If the ball hits bottom of screen
        if (hit.HitNormal.x == 0 && hit.HitNormal.y == -1) {
            // Destroy
            isWaitingDestroy = true;
            return;
        }

        // Reflect and calculate velocity
        ReflectBall(hit);
        CalculateDirection(physics->maxSpeed);
        return;
    }

    // If ball hits another ball (Currently not used)
    if (hit.otherTag == "Ball") {
        ReflectBall(hit);
        CalculateDirection(physics->maxSpeed);

        
        return;
    }

    // If no case above is met, the ball has hit a paddle


    PlaySound(PaddleHitSound);

    // Get hit point on surface of paddle
    float facePos = hit.percentDistanceAlongHitFace;
    float speedMultiplier = 1;



    // If ball is moving left
    if (movementDirection.x < 0) {
        // If ball lands on right side of paddle
        if ((facePos >= 0.5 && hit.otherTag == "Player") || hit.otherTag == "RightPlayerEnd") {
            if (hit.otherTag == "Player") {
                // Convert facePos range from 0-0.5 to 0.5-1 to use for speed multiplier
                speedMultiplier = ((facePos - 0.5f) / (1.0f - 0.5f)) * (1.2f - 0.8f) + 0.8f;
                // Vary the x direction based on where it hit the paddle
                movementDirection.x = -1 * speedMultiplier;
            }
            
            ReturnBall(hit);
        }
        // If ball lands on left side of paddle
        else {
            if (hit.otherTag == "Player") {
                // Convert facePos range from 0-0.5 to 0.5-1 to use for speed multiplier
                speedMultiplier = ((facePos - 0) / (0.5f - 0)) * (1.2f - 0.8f) + 0.8f;
                // Vary the x direction based on where it hit the paddle
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
                // Vary the x direction based on where it hit the paddle
                movementDirection.x = 1 * speedMultiplier;
            }
            ReturnBall(hit);
        }
        // If ball lands on right side of paddle
        else {
            if (hit.otherTag == "Player") {
                // Convert facePos range from 0-0.5 to 0.5-1 to use for speed multiplier
                speedMultiplier = ((facePos - 0.5f) / (1.0f - 0.5f)) * (1.2f - 0.8f) + 0.8f;

                // Vary the x direction based on where it hit the paddle
                movementDirection.x = 1 * speedMultiplier;
            }
            ReflectBall(hit);
        }
        
        // Calculate velocity
        CalculateDirection(physics->maxSpeed * speedMultiplier);
        return;
    }

}


