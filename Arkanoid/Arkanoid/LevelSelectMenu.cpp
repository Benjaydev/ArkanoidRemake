#include "LevelSelectMenu.h"
#include "Map.h"

LevelSelectMenu::LevelSelectMenu(float x, float y)
{
	physics->SetPosition(x, y);

	backPanel = new UIPanel(0, 0, 500, 700, 0xEEEEEECC);
	AddChild(backPanel);
	Vector2 panelOffset = backPanel->sprite->GetCentreOffset();
	backPanel->physics->SetPosition(panelOffset.x, panelOffset.y);

	confirmButton = new UIButton(0, 0, 100, 50, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Confirm", 16, 0xFFFFFFFF));
	backPanel->AddChild(confirmButton);


	Map map = Map();

	// Add button for each level
	for (int i = 0; i < map.GetMapCount()-1; i++) {
		UIButton* b = new UIButton(0,0, 450, 50, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, map.GetMapName(i), 16, 0xFFFFFFFF));
		backPanel->AddChild(b);
		b->AssignCallMethod(std::bind(&LevelSelectMenu::ChooseIndex, this, i));
		b->physics->SetPosition(250, 50+(60 * i));
		// Move confirm button down below buttons
		confirmButton->physics->SetPosition(250, 50 + (60 * (i+1)));
		mapButtons.push_back(b);
	}
}

LevelSelectMenu::~LevelSelectMenu()
{
	backPanel->isWaitingDestroy = true;
	confirmButton->isWaitingDestroy = true;
	for (int i = 0; i < mapButtons.size(); i++) {
		mapButtons[i]->isWaitingDestroy = true;
	}
}

void LevelSelectMenu::ChooseIndex(int index)
{
	chosenIndex = index;
	// Select colour
	mapButtons[index]->defaultColour = 0xAAAAFFFF;
	mapButtons[index]->hoverColour = 0xAAAAFFFF;

	// Return every other button that is not selected to the default
	for (int i = 0; i < mapButtons.size(); i++) {
		if (i == index) {
			continue;
		}
		mapButtons[i]->defaultColour = 0x585858FF;
		mapButtons[i]->hoverColour = ColorToInt(BLUE);
	}


}
