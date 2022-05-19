#pragma once
#include "UIObject.h"
#include "Map.h"
#include "UIPanel.h"
#include "UIText.h"
#include "UIInputBox.h"
#include "UIButton.h"
#include "LevelSelectMenu.h"
class LevelEditor : public UIObject
{
public:
	LevelEditor();
	virtual ~LevelEditor();

	Map map = Map();

	UIObject* cursorBrick;
	UIObject* brickSettingDisplay;

	BrickStruct currentBrickStruct = BrickStruct();


	UIPanel* optionsPanel;
	UIText* optionsPanelText;

	// Level
	UIText* optionsPanelLevelText;
	UIInputBox* levelNameInput;
	UIText* levelNameText;

	UIButton* saveButton;
	UIButton* overrideButton;
	UIButton* loadButton;
	UIButton* newButton;

	UIButton* mainMenuButton;

	// Background
	UIText* backgroundChooseText;
	UIInputBox* redInputBoxBack;
	UIText* redTextBack;
	UIInputBox* greenInputBoxBack;
	UIText* greenTextBack;
	UIInputBox* blueInputBoxBack;
	UIText* blueTextBack;



	// Brick
	UIInputBox* healthInputBox;
	UIText* healthText;

	UIInputBox* redInputBoxBrick;
	UIText* redTextBrick;
	UIInputBox* greenInputBoxBrick;
	UIText* greenTextBrick;
	UIInputBox* blueInputBoxBrick;
	UIText* blueTextBrick;


	UIObject* bricks[130];

	LevelSelectMenu* levelSelectMenu;
	bool levelSelectOpen = false;

	float cooldown = 0;
	bool isErasing = false;

	void Update(float DeltaTime) override;

	void SaveLevel();
	void OverrideLevel();
	void NewLevel();
	void ToggleLoadMenu();
	void OpenLevel();

};

