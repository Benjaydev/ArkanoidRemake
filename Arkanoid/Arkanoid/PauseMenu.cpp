#pragma once
#include "PauseMenu.h"
#include "Game.h"
PauseMenu::PauseMenu(float x, float y)
{
	physics->SetPosition(x, y);

	// Create main back panel for all elements
	mainPanel = new UIPanel(0, 0, 500, 600, 0xEEEEEE88);
	AddChild(mainPanel);
	Vector2 panelOffset = mainPanel->sprite->GetCentreOffset();
	mainPanel->physics->SetPosition(panelOffset.x, panelOffset.y);

	// Add header text
	headerText = new UIText(0, 0, "Paused", 80, 0x000000FF);
	AddChild(headerText);
	Vector2 headerOffset = headerText->GetCentreOffset();
	headerText->physics->SetPosition(headerOffset.x, panelOffset.y);

	
	// Add resume button
	resumeButton = new UIButton(0, 0, 200, 100, 0x585858FF, new UIText(0, 0, "Resume", 16, 0xFFFFFFFF));
	AddChild(resumeButton);
	resumeButton->physics->SetPosition(0, -100);


	AddToGameWorld();

}

PauseMenu::~PauseMenu()
{
	mainPanel->isWaitingDestroy = true;
	headerText->isWaitingDestroy = true;
	resumeButton->isWaitingDestroy = true;
}
