#include "Collider.h"

Collider::Collider()
{
}

Collider::Collider(Collider* copy)
{
}

Collider::~Collider()
{
}

Collider* Collider::Copy(Collider* copy)
{
	if (copy->type == cType::Rectangle) {
		return new RectangleCollider((RectangleCollider*)copy);
	}
	else if (copy->type == cType::Circle) {
		return new CircleCollider((CircleCollider*)copy);
	}
}
