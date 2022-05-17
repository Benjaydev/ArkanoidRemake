#include "MainMenu.h"

MainMenu::MainMenu()
{
	backgroundPanel = new UIPanel(0, 0, GetScreenWidth(), GetScreenHeight(), 0xEEEEEEFF);
	AddChild(backgroundPanel);


	MenuText = new UIText(0, 0, "Arkenoid", 80, 0x000000FF);
	backgroundPanel->AddChild(MenuText);
	MenuText->physics->SetPosition(100, 50);
	

	startGameButton = new UIButton(0, 0, 200, 100, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Play Game", 20, 0xFFFFFFFF));
	backgroundPanel->AddChild(startGameButton);
	startGameButton->physics->SetPosition(200, 250);

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
	startGameButton->isWaitingDestroy = true;
	levelEditButton->isWaitingDestroy = true;
	exitButton->isWaitingDestroy = true;
}
