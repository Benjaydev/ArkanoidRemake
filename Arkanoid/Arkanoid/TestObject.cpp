#include "TestObject.h"

TestObject::TestObject(float x, float y) {
    
    parent->LoadSprite((char*)"TestImage.png");
    parent->sprite->SetScale(0.1);

    parent->physics->SetPosition(x, y);

    child->LoadSprite((char*)"TestImage.png");
    child->sprite->SetScale(0.25);

    parent->physics->SetCollider(cType::Rectangle);
    parent->physics->FitColliderWH(child->sprite->GetWidth(), child->sprite->GetHeight());

    parent->AddChild(child);
    child->physics->SetPosition(child->sprite->GetCentreOffset());
    parent->AddToGameWorld();

    

}
TestObject::~TestObject() {
    delete parent;
    delete child;
}