#pragma once
#include "UIPanel.h"

UIPanel::UIPanel(float x, float y, float width, float height, int colour)
{
	physics->SetPosition(x, y);
	LoadSprite((char*)"WhitePixel.png");
	sprite->texture->width = width;
	sprite->texture->height = height;
	sprite->colour = GetColor(colour);


	AddToGameWorld();

}

UIPanel::~UIPanel()
{
}