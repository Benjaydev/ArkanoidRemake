#pragma once
#include "UIObject.h"
#include "UIText.h"
#include "UIPanel.h"
#include <functional>

class UIButton : public UIObject
{
public:
	UIButton(float x, float y, float width, float height, int colour, UIText* text);
	~UIButton();

	std::function<void()> callFunction;

	UIPanel* buttonSprite;
	UIText* buttonText;

	void OnClick();
	void AssignCallMethod(std::function<void()> callMethod) { callFunction = callMethod; };
};

