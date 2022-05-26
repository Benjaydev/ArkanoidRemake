#include "MainMenu.h"
#include "Game.h"

MainMenu::MainMenu()
{
	backgroundPanel = new UIPanel(0, 0, GetScreenWidth(), GetScreenHeight(), 0xEEEEFFAA);
	AddChild(backgroundPanel);

	// Setup title text
	menuText = new UIText(0, 0, "Arkanoid", 80, 0x000000FF);
	backgroundPanel->AddChild(menuText);
	menuText->physics->SetPosition(100, 50);	
	new UIText(100, 125, "By Ben Wharton", 25, 0x000000FF);

	// Display the chosen level
	chosenLevelText = new UIText(0, 0, "None", 24, 0x000000FF);
	backgroundPanel->AddChild(chosenLevelText);
	chosenLevelText->physics->SetPosition(460, 212.5);

	// Create start game button
	startGameButton = new UIButton(0, 0, 200, 100, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Play Game", 20, 0xFFFFFFFF));
	backgroundPanel->AddChild(startGameButton);
	startGameButton->physics->SetPosition(200, 250);
	// Default not active as no level is selected
	startGameButton->isActive = false;
	
	// Create story mode button
	storyModeButton = new UIButton(0, 0, 150, 45, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Story", 20, 0xFFFFFFFF));
	backgroundPanel->AddChild(storyModeButton);
	storyModeButton->physics->SetPosition(380, 277.5);

	// Crete load save button
	loadSaveButton = new UIButton(0, 0, 150, 45, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Choose Level", 20, 0xFFFFFFFF));
	backgroundPanel->AddChild(startGameButton);
	loadSaveButton->physics->SetPosition(380, 222.5);
	loadSaveButton->AssignCallMethod(std::bind(&MainMenu::ToggleLoadMenu, this));

	// Create level edit button
	levelEditButton = new UIButton(0, 0, 200, 100, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Level Editor", 20, 0xFFFFFFFF));
	backgroundPanel->AddChild(levelEditButton);
	levelEditButton->physics->SetPosition(200, 450);

	// Create exit button
	exitButton = new UIButton(0, 0, 200, 100, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Exit", 20, 0xFFFFFFFF));
	backgroundPanel->AddChild(exitButton);
	exitButton->physics->SetPosition(200, 650);
	exitButton->AssignCallMethod(std::bind(&Game::CloseGameWindow));


	AddToGameWorld();
}

MainMenu::~MainMenu()
{
	// Destroy all
	backgroundPanel->isWaitingDestroy = true;
	menuText->isWaitingDestroy = true;
	chosenLevelText->isWaitingDestroy = true;

	if (levelSelectMenu != nullptr) {
		levelSelectMenu->isWaitingDestroy = true;
	}
	
	startGameButton->isWaitingDestroy = true;
	levelEditButton->isWaitingDestroy = true;
	loadSaveButton->isWaitingDestroy = true;
	exitButton->isWaitingDestroy = true;
}


void MainMenu::ToggleLoadMenu() {
	levelSelectOpen = !levelSelectOpen;

	// If level select menu should close
	if (!levelSelectOpen) {
		loadSaveButton->isActive = true;
		storyModeButton->isActive = true;
		levelSelectMenu->isWaitingDestroy = true;
		return;
	}
	// Make play buttons unactive if menu is open
	loadSaveButton->isActive = false;
	storyModeButton->isActive = false;

	// Create new level select menu
	levelSelectMenu = new LevelSelectMenu((GetScreenWidth() / 2), GetScreenHeight() / 2);
	levelSelectMenu->confirmButton->AssignCallMethod(std::bind(&MainMenu::ChooseLevel, this));
}

void MainMenu::ChooseLevel() {
	if (levelSelectMenu->chosenIndex == -1) {
		return;
	}
	ToggleLoadMenu();

	startGameButton->isActive = true;
	// Set start game button to open chosen map on click
	startGameButton->AssignCallMethod(std::bind(&Game::StartGame, Game::ThisGame, levelSelectMenu->chosenIndex));

	Map map = Map();
	chosenLevelText->text = map.GetMapName(levelSelectMenu->chosenIndex);
	
	
}