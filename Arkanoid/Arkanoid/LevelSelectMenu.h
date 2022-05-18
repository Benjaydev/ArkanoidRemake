#pragma once
#include "UIPanel.h"
#include "UIText.h"
#include "UIButton.h"
class LevelSelectMenu : public UIObject
{
public:
	LevelSelectMenu(float x, float y);
	virtual ~LevelSelectMenu();

	UIPanel* backPanel;
	UIText* title;

	UIButton* confirmButton;

	int chosenIndex = -1;

	std::vector<std::string> maps;

	std::vector<UIButton*> mapButtons;
	

	void ChooseIndex(int index);



};

