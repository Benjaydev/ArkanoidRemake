#pragma once
#include "SpriteComponent.h"

SpriteComponent::~SpriteComponent()
{
	UnloadImage(*image);
	delete image;
	image = nullptr;

	UnloadTexture(*texture);
	delete texture;
	texture = nullptr;
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