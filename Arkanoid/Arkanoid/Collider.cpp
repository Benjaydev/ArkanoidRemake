#include "Collider.h"
#include <iostream>
using namespace std;

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

bool Collider::OverlapsScreen(Vector3 velocity, Hit& result)
{

	if (type == cType::Rectangle) {
		RectangleCollider* rec = (RectangleCollider*)this;
		Vector2 screen = { GetScreenWidth(), GetScreenHeight() };

		result.OutVel = velocity;

		if (rec->min.x + velocity.x < 0) {
			result.HitNormal.x = 1;
			result.OutVel.x = velocity.x - (rec->min.x + velocity.x);
		}
		if (rec->min.y + velocity.y < 0) {
			result.HitNormal.y = 1;
			result.OutVel.y = velocity.y - (rec->min.y + velocity.y);
		}
		if (rec->max.x + velocity.x > screen.x) {
			result.HitNormal.x = -1;
			result.OutVel.x = velocity.x + (screen.x - (rec->max.x + velocity.x));
		}
		if (rec->max.y+ velocity.y > screen.y) {
			result.HitNormal.y = -1;
			result.OutVel.y = velocity.y + (screen.y - (rec->max.y + velocity.y));
		}


		if (result.HitNormal.x != 0 || result.HitNormal.y != 0) {
			return true;
		}
	}
	else if (type == cType::Circle) {
		CircleCollider* cir = (CircleCollider*)this;
		Vector2 screen = { GetScreenWidth(), GetScreenHeight() };

		result.OutVel = velocity;

		Vector3 normVelocity = Vector3Normalize(velocity);


		if ((cir->center.x + velocity.x) + normVelocity.x*cir->radius < 0) {
			result.HitNormal.x = 1;
			result.OutVel.x =  (velocity.x - (cir->center.x + velocity.x)) - (normVelocity.x * cir->radius);
		}
		if ((cir->center.y + velocity.y) + normVelocity.y * cir->radius < 0) {
			result.HitNormal.y = 1;
			result.OutVel.y =  (velocity.y - (cir->center.y + velocity.y)) - (normVelocity.y * cir->radius);
		}
		if ((cir->center.x + velocity.x) + normVelocity.x * cir->radius > screen.x) {
			result.HitNormal.x = -1;
			
			result.OutVel.x =  (velocity.x + (screen.x - (cir->center.x + velocity.x))) - (normVelocity.x * cir->radius);

			std::cout << velocity.x << ", " << result.OutVel.x << std::endl;
		}
		if ((cir->center.y + velocity.y) + normVelocity.y * cir->radius > screen.y) {
			result.HitNormal.y = -1;
			result.OutVel.y = (velocity.y + (screen.y - (cir->center.y + velocity.y))) - (normVelocity.y * cir->radius);
		}


		if (result.HitNormal.x != 0 || result.HitNormal.y != 0) {
			return true;
		}
	}


	return false;
}
