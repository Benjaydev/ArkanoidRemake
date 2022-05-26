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

	UIButton* confirmButton;
	UIText* controlsText;

	int chosenIndex = -1;

	int lowerIndex = 0;
	int upperIndex = 0;

	int mapCount = 0;

	std::vector<std::string> maps;

	std::vector<UIButton*> mapButtons;
	

	void ChooseIndex(int index);

	void Update(float DeltaTime);

};

