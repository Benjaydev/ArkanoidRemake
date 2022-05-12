#include "TestObject.h"

TestObject::TestObject(float x, float y, cType type) {
    
    float size = (rand() % 25 + 1) / 100.0f;
    parent->LoadSprite((char*)"TestImage.png");
    parent->sprite->SetScale(0.1);

    parent->physics->SetPosition(x, y);

    child->LoadSprite((char*)"TestImage.png");
    child->sprite->SetScale(0.2);

    parent->physics->SetCollider(type);
    parent->physics->FitColliderWH(child->sprite->GetWidth(), child->sprite->GetHeight(), { parent->physics->globalTransform.m8, parent->physics->globalTransform.m9 });

    parent->AddChild(child);
    child->physics->SetPosition(child->sprite->GetCentreOffset());
    parent->AddToGameWorld();

    int speed = rand() % 250 + 50;
    parent->physics->moveSpeed = speed;
    parent->physics->maxSpeed = speed;

}
TestObject::~TestObject() {
    delete parent;
    delete child;
}