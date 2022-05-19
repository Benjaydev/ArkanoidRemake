#pragma once
#include "UIPanel.h"
#include "UIButton.h"
#include "UIText.h"
#include "LevelSelectMenu.h"

class MainMenu : public UIObject
{
public:
	MainMenu();
	virtual ~MainMenu();

	UIPanel* backgroundPanel;

	UIText* MenuText;

	UIButton* startGameButton;
	UIButton* loadSaveButton;
	UIButton* levelEditButton;
	UIButton* exitButton;

	UIText* chosenLevelText;

	LevelSelectMenu* levelSelectMenu;

	bool levelSelectOpen = false;
	void ToggleLoadMenu();
	void ChooseLevel();
};
