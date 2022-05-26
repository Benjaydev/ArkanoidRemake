#include "LevelSelectMenu.h"
#include "Map.h"

LevelSelectMenu::LevelSelectMenu(float x, float y)
{
	physics->SetPosition(x, y);

	// Setup the background
	backPanel = new UIPanel(0, 0, 500, 700, 0xEEEEEECC);
	AddChild(backPanel);
	Vector2 panelOffset = backPanel->sprite->GetCentreOffset();
	backPanel->physics->SetPosition(panelOffset.x, panelOffset.y);

	// Create confirm button
	confirmButton = new UIButton(0, 0, 100, 50, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Confirm", 16, 0xFFFFFFFF));
	backPanel->AddChild(confirmButton);


	Map map = Map();
	mapCount = map.GetMapCount();

	// Get the lower and upper indices (Maps between these indices will be displayed)
	lowerIndex = 0;
	upperIndex = fminf(9, mapCount);


	// Add button for each level
	for (int i = 0; i < map.GetMapCount(); i++) {
		// Create name (Format - Index: Name)
		std::string name = std::to_string(i) + ": " + map.GetMapName(i);
		// Create button
		UIButton* b = new UIButton(0,0, 450, 50, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, name, 16, 0xFFFFFFFF));
		backPanel->AddChild(b);
		// Assign the function call with map index
		b->AssignCallMethod(std::bind(&LevelSelectMenu::ChooseIndex, this, i));
		// Move button up y axis the higher the index
		b->physics->SetPosition(250, 50+(60 * i));
		
		mapButtons.push_back(b);
	}
	// Move confirm button down below buttons
	confirmButton->physics->SetPosition(250, 50 + (60 * (map.GetMapCount())));

	// Show controls
	controlsText = new UIText(0, 0, " Scroll [ W: Up | S: Down ]", 16, 0x000000FF);
	backPanel->AddChild(controlsText);
	controlsText->physics->SetPosition(0, 680);

}

LevelSelectMenu::~LevelSelectMenu()
{
	// Destroy all
	backPanel->isWaitingDestroy = true;
	confirmButton->isWaitingDestroy = true;
	controlsText->isWaitingDestroy = true;

	// Destroy each map select button
	for (int i = 0; i < mapButtons.size(); i++) {
		mapButtons[i]->isWaitingDestroy = true;
	}
}

void LevelSelectMenu::ChooseIndex(int index)
{
	chosenIndex = index;
	// Selected colour
	mapButtons[index]->defaultColour = 0xAAAAFFFF;
	mapButtons[index]->hoverColour = 0xAAAAFFFF;

	// Return every other button that is not selected to the default colour
	for (int i = 0; i < mapButtons.size(); i++) {
		if (i == index) {
			continue;
		}
		mapButtons[i]->defaultColour = 0x585858FF;
		mapButtons[i]->hoverColour = ColorToInt(BLUE);
	}


}

void LevelSelectMenu::Update(float DeltaTime)
{
	// Scroll downwards
	if (IsKeyPressed(KEY_S)) {
		// Keep upper index within map count
		upperIndex = fminf(upperIndex + 1, mapCount-1);

		// Set lower index to 9 below upper index clamped above or equal to 0
		lowerIndex = fmaxf(upperIndex - 9, 0);
	}
	// Scroll upwards
	else if (IsKeyPressed(KEY_W)) {
		// Keep upper index equal to or above mapcount (or 9 if mapcount is greater than 9)
		upperIndex = fmaxf(upperIndex - 1, fminf(mapCount-1, 9));

		// Lower index is 9 less than upper index
		lowerIndex = upperIndex-9;

	}


	// See how much upper index goes above 9
	int diff = 9 - upperIndex;
	// If upper index doesn't go above 9, set diff to 0
	diff = diff < 0 ? diff : 0;

	// Set positions of buttons
	for (int i = 0; i < mapCount; i++) {
		if (i <= upperIndex && i >= lowerIndex) {
			// Place wanted buttons at locations from 0-9
			mapButtons[i]->physics->SetPosition(250, 50 + (60 * (i+diff)));
			// Move confirm button down below buttons
			confirmButton->physics->SetPosition(250, 50 + (60 * ((i + diff) + 1)));
		}
		else {
			// Move unwanted buttons to outside of screen
			mapButtons[i]->physics->SetPosition(-2000,-2000);
		}	

		
	}

}
