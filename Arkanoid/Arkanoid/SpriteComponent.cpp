#pragma once
#include "SpriteComponent.h"


SpriteComponent::~SpriteComponent()
{
	delete image;
	delete texture;
}

void SpriteComponent::Load(char* filename)
{
	*image = LoadImage(filename);
	*texture = LoadTextureFromImage(*image);

	defaultWidth = texture->width;
	defaultHeight = texture->height;
}


void SpriteComponent::SetScale(float scale) {
	textureScale = scale;
	texture->width *= scale;
	texture->height *= scale;
}