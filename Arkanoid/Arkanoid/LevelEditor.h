#pragma once
#include "UIObject.h"
#include "Map.h"
#include "UIPanel.h"
#include "UIText.h"
#include "UIInputBox.h"
#include "UIButton.h"
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
	//UIButton* loadButton;


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



	void Update(float DeltaTime) override;
	void SaveLevel();

};

