#pragma once
#include "UIObject.h"
#include "UIText.h"
#include "UIPanel.h"
class UIButton : public UIObject
{
public:
	UIButton(float x, float y, float width, float height, int colour, UIText* text, void (*callMethod)() = nullptr);
	~UIButton();

	void (*callFunction)();

	UIPanel* buttonSprite;
	UIText* buttonText;

	void OnClick();
};

