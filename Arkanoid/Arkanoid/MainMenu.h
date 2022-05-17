#pragma once
#include "UIPanel.h"
#include "UIButton.h"
#include "UIText.h"

class MainMenu : public UIObject
{
public:
	MainMenu();
	~MainMenu();

	UIPanel* backgroundPanel;

	UIText* MenuText;

	UIButton* startGameButton;
	UIButton* levelEditButton;
	UIButton* exitButton;

};

