#pragma once
#include "raylib.h";
#include <string>
struct Hit {
	Vector3 OutVel = Vector3();
	Vector2 HitNormal = Vector2();
	float percentDistanceAlongHitFace = -1;
	std::string otherTag = "";
	
};

