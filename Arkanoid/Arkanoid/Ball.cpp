#include "Ball.h"
#include "Hit.h"

Ball::Ball(float x, float y)
{
    tag = "Ball";


    physics->SetPosition(x, y);
    ballSprite ->LoadSprite((char*)"Ball.png");
    ballSprite->sprite->SetScale(4);

    AddChild(ballSprite);

    Vector2 ballPos = { physics->globalTransform->m8,physics->globalTransform->m9 };
    physics->SetCollider(cType::Circle);
    physics->FitColliderWH(ballSprite->sprite->GetWidth()/2, ballSprite->sprite->GetHeight()/2, ballPos);


    Vector2 offset = ballSprite->sprite->GetCentreOffset();

    ballSprite->physics->SetPosition(offset.x, offset.y);
    AddToGameWorld();


    physics->moveSpeed = 500;
    physics->maxSpeed = 500;
    physics->deceleration = 1;
}

Ball::~Ball()
{
    delete ballSprite;
}

void Ball::Update(float DeltaTime)
{
    Object::Update(DeltaTime);

    CalculateDirection(physics->maxSpeed);
}
void Ball::CalculateDirection(float speed)
{
    Vector2 normDir = Vector2Normalize(movementDirection);
    physics->velocity->x = normDir.x * speed;
    physics->velocity->y = normDir.y * speed;
}

void Ball::ReflectBall(Hit hit)
{
    Vector2 normalDir = Vector2(movementDirection);
    normalDir.x *= hit.HitNormal.x;
    normalDir.y *= hit.HitNormal.y;
    
    normalDir.x *= 2;
    normalDir.y *= 2;

    normalDir.x *= hit.HitNormal.x;
    normalDir.y *= hit.HitNormal.y;


    movementDirection = Vector2Subtract(movementDirection, normalDir);

}



void Ball::ReturnBall(Hit hit)
{
    movementDirection = Vector2Negate(movementDirection);
}


void Ball::CollideEvent(Hit hit)
{
    if (hit.otherTag == "Brick") {
        ReflectBall(hit);
        CalculateDirection(physics->maxSpeed);
        return;
    }
    if (hit.otherTag == "Wall") {
        // If the ball hits bottom of screen
        if (hit.HitNormal.x == 0 && hit.HitNormal.y == -1) {
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
    if ((hit.HitNormal.x != 0 && hit.HitNormal.y != -1)) {
        
        ReflectBall(hit);
        CalculateDirection(physics->maxSpeed*20);
        return;
    }

    std::cout << hit.HitNormal.x << ", " << hit.HitNormal.y << std::endl;

   
    float facePos = hit.percentDistanceAlongHitFace;

    // If ball is moving left
    if (movementDirection.x < 0) {
        // If ball lands on right side of paddle
        if (facePos >= 0.5) {
            ReturnBall(hit);
        }
        // If ball lands on left side of paddle
        else {
            ReflectBall(hit);
        }
        CalculateDirection(physics->maxSpeed);
        return;
    }
    // If ball is moving right
    else if (movementDirection.x > 0) {
        // If ball lands on left side of paddle
        if (facePos <= 0.5) {
            ReturnBall(hit);
        }
        // If ball lands on right side of paddle
        else {
            ReflectBall(hit);
        }
        CalculateDirection(physics->maxSpeed);
        return;
    }


    ReflectBall(hit);
    CalculateDirection(physics->maxSpeed);
}


