#pragma once
#include "UIPanel.h"
#include "UIText.h"
#include "UIButton.h"
class PauseMenu : public UIObject
{
public:
	PauseMenu(float x, float y);
	PauseMenu(){};
	virtual ~PauseMenu();

	
	UIPanel* mainPanel;
	UIText* headerText;

	UIButton* resumeButton;
	UIButton* mainmenuButton;

};