#include "SpriteObject.h"

SpriteObject::SpriteObject()
{
}

SpriteObject::~SpriteObject()
{
}

void SpriteObject::Load(char* filename)
{
	*image = LoadImage(filename);
	*texture = LoadTextureFromImage(*image);

}

void SpriteObject::OnDraw()
{

	float rotation = (float)atan2(globalTransform->m1, globalTransform->m0);

	if (!WindowShouldClose) {
		Vector2 position = { globalTransform->m8, globalTransform->m9 };
		// Draw sprite to screen
		DrawTextureEx(*texture, position, rotation * RAD2DEG, textureScale, colour);
	}

}



