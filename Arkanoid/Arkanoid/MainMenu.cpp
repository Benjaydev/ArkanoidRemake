#include "MainMenu.h"
#include "Game.h"

MainMenu::MainMenu()
{
	backgroundPanel = new UIPanel(0, 0, GetScreenWidth(), GetScreenHeight(), 0xEEEEEEAA);
	AddChild(backgroundPanel);


	MenuText = new UIText(0, 0, "Arkenoid", 80, 0x000000FF);
	backgroundPanel->AddChild(MenuText);
	MenuText->physics->SetPosition(100, 50);	
	
	
	chosenLevelText = new UIText(0, 0, "None", 24, 0x000000FF);
	backgroundPanel->AddChild(chosenLevelText);
	chosenLevelText->physics->SetPosition(280, 320);



	

	startGameButton = new UIButton(0, 0, 200, 100, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Play Game", 20, 0xFFFFFFFF));
	backgroundPanel->AddChild(startGameButton);
	startGameButton->physics->SetPosition(200, 250);
	startGameButton->isActive = false;
	
	loadSaveButton = new UIButton(0, 0, 150, 50, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Choose Level", 20, 0xFFFFFFFF));
	backgroundPanel->AddChild(startGameButton);
	loadSaveButton->physics->SetPosition(200, 330);
	loadSaveButton->AssignCallMethod(std::bind(&MainMenu::ToggleLoadMenu, this));

	levelEditButton = new UIButton(0, 0, 200, 100, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Level Editor", 20, 0xFFFFFFFF));
	backgroundPanel->AddChild(levelEditButton);
	levelEditButton->physics->SetPosition(200, 450);

	exitButton = new UIButton(0, 0, 200, 100, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Exit", 20, 0xFFFFFFFF));
	backgroundPanel->AddChild(exitButton);
	exitButton->physics->SetPosition(200, 650);



	AddToGameWorld();
}

MainMenu::~MainMenu()
{
	backgroundPanel->isWaitingDestroy = true;
	MenuText->isWaitingDestroy = true;
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

	if (!levelSelectOpen) {
		levelSelectMenu->isWaitingDestroy = true;
		return;
	}

	levelSelectMenu = new LevelSelectMenu(GetScreenWidth() / 2, GetScreenHeight() / 2);
	levelSelectMenu->confirmButton->AssignCallMethod(std::bind(&MainMenu::ChooseLevel, this));
}

void MainMenu::ChooseLevel() {
	levelSelectOpen = !levelSelectOpen;
	levelSelectMenu->isWaitingDestroy = true;
	startGameButton->isActive = true;
	startGameButton->AssignCallMethod(std::bind(&Game::StartGame, Game::ThisGame, levelSelectMenu->chosenIndex));

	Map map = Map();
	chosenLevelText->text = map.GetMapName(levelSelectMenu->chosenIndex);
}