#include "UIObject.h"

UIObject::UIObject()
{
	tag = "UI";
	physics->LockAxis = { 1, 1 };
	physics->hasPhysicsCheck = false;


	AddToGameWorld();

}
UIObject::~UIObject()
{
}
