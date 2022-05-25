#include "LevelEditor.h"
#include "Game.h"
LevelEditor::LevelEditor()
{
	cooldown = 1;
	cursorBrick = new UIObject();
	LoadSprite((char*)"TestBackground2.png");
	physics->SetPosition(Game::WorldBorders.x, Game::WorldBorders.y);
	sprite->colour = GetColor((int)0xFFFFFF66);
	cursorBrick->LoadSprite((char*)"Brick.png");

	
	optionsPanel = new UIPanel(0, GetScreenHeight()-300, GetScreenWidth(), 300, 0xFAEBD7DD);
	optionsPanelText = new UIText(GetScreenWidth()-350, GetScreenHeight() - 300, "Brick Settings", 40, 0x000000FF);
	optionsPanelLevelText = new UIText(10, GetScreenHeight() - 300, "Level Settings", 40, 0x000000FF);

	levelNameInput = new UIInputBox(125, GetScreenHeight() - 215, 200, 40, 0x000000FF, ColorToInt(BLUE), 0x8888FFFF, new UIText(0, 0, "New Level", 16, 0xFFFFFFFF), 20);
	levelNameText = new UIText(25, GetScreenHeight() - 260, "Level Name", 24, 0x000000FF);

	saveButton = new UIButton(75, GetScreenHeight() - 70, 100, 40, 0x585858FF, ColorToInt(GREEN), new UIText(0, 0, "Save", 16, 0xFFFFFFFF));
	saveButton->AssignCallMethod(std::bind(&LevelEditor::SaveLevel, this));

	overrideButton = new UIButton(75, GetScreenHeight() - 25, 100, 30, 0x585858FF, ColorToInt(RED), new UIText(0, 0, "Override", 16, 0xFFFFFFFF));
	overrideButton->AssignCallMethod(std::bind(&LevelEditor::OverrideLevel, this));
	overrideButton->isActive = false;

	loadButton = new UIButton(200, GetScreenHeight() - 70, 100, 40, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Load", 16, 0xFFFFFFFF));
	loadButton->AssignCallMethod(std::bind(&LevelEditor::ToggleLoadMenu, this));
	
	newButton = new UIButton(200, GetScreenHeight() - 25, 100, 30, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "New", 16, 0xFFFFFFFF));
	newButton->AssignCallMethod(std::bind(&LevelEditor::NewLevel, this));
	
	
	mainMenuButton = new UIButton(310, GetScreenHeight() - 50, 100, 40, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Main Menu", 16, 0xFFFFFFFF));


	new UIText(70, GetScreenHeight() - 320, "Left Mouse Button: Place Brick | Right Mouse Button: Erase Brick | Middle Mouse Button: Color picker", 16, 0xFFFFFFFF);


	// Background colour choose
	Vector2 backCCPos = { 55,  GetScreenHeight() - 130 };
	// Red
	redInputBoxBack = new UIInputBox(backCCPos.x, backCCPos.y, 60, 30, 0x000000FF, ColorToInt(RED), 0xFF8888FF, new UIText(0, 0, "55", 16, 0xFFFFFFFF), 3, true);
	redInputBoxBack->intMax = 255;
	redInputBoxBack->intMin = 0;
	redTextBack = new UIText(backCCPos.x-25, backCCPos.y-30, "Red", 16, 0x000000FF);

	// Green
	greenInputBoxBack = new UIInputBox(backCCPos.x+70, backCCPos.y, 60, 30, 0x000000FF, ColorToInt(GREEN), 0x88FF88FF, new UIText(0, 0, "55", 16, 0xFFFFFFFF), 3, true);
	greenInputBoxBack->intMax = 255;
	greenInputBoxBack->intMin = 0;
	greenTextBack = new UIText(backCCPos.x + 70 - 25, backCCPos.y - 30, "Green", 16, 0x000000FF);

	// Blue
	blueInputBoxBack = new UIInputBox(backCCPos.x+140, backCCPos.y, 60, 30, 0x000000FF, ColorToInt(BLUE), 0x8888FFFF, new UIText(0, 0, "55", 16, 0xFFFFFFFF), 3, true);
	blueInputBoxBack->intMax = 255;
	blueInputBoxBack->intMin = 0;
	blueTextBack = new UIText(backCCPos.x + 140 - 25, backCCPos.y -  30, "Blue", 16, 0x000000FF);

	backgroundChooseText = new UIText(backCCPos.x-30, backCCPos.y - 55, "Background Colour", 24, 0x000000FF);




	// Brick colour choose
	Vector2 brickCCPos = { GetScreenWidth() - 200,  GetScreenHeight() - 225 };
	// Red
	redInputBoxBrick = new UIInputBox(brickCCPos.x, brickCCPos.y, 80, 40, 0x000000FF, ColorToInt(RED), 0xFF8888FF, new UIText(0, 0, "255", 16, 0xFFFFFFFF), 3, true);
	redInputBoxBrick->intMax = 255;
	redInputBoxBrick->intMin = 0;
	redTextBrick = new UIText(brickCCPos.x-100,brickCCPos.y-10, "Red", 24, 0x000000FF);

	// Green
	greenInputBoxBrick = new UIInputBox(brickCCPos.x, brickCCPos.y+50, 80, 40, 0x000000FF, ColorToInt(GREEN), 0x88FF88FF, new UIText(0, 0, "255", 16, 0xFFFFFFFF), 3, true);
	greenInputBoxBrick->intMax = 255;
	greenInputBoxBrick->intMin = 0;
	greenTextBrick = new UIText(brickCCPos.x-120, brickCCPos.y-10+50, "Green", 24, 0x000000FF);

	// Blue
	blueInputBoxBrick = new UIInputBox(brickCCPos.x, brickCCPos.y+100, 80, 40, 0x000000FF, ColorToInt(BLUE), 0x8888FFFF, new UIText(0, 0, "255", 16, 0xFFFFFFFF), 3, true);
	blueInputBoxBrick->intMax = 255;
	blueInputBoxBrick->intMin = 0;
	blueTextBrick = new UIText(brickCCPos.x-100, brickCCPos.y-10+100, "Blue", 24, 0x000000FF);

	brickSettingDisplay = new UIObject();
	brickSettingDisplay->LoadSprite((char*)"Brick.png");
	brickSettingDisplay->physics->SetPosition(brickCCPos.x + 100, brickCCPos.y - 10 + 50);
	brickSettingDisplay->AddToGameWorld();



	// Brick health input
	healthInputBox = new UIInputBox(GetScreenWidth()-100, GetScreenHeight() - 50, 40, 40, 0x000000FF, ColorToInt(BLUE), 0x8888FFFF, new UIText(0, 0, "1", 16, 0xFFFFFFFF), 1, true);
	healthInputBox->intMax = 9;
	healthInputBox->intMin = 1;
	healthText = new UIText(GetScreenWidth() - 210, GetScreenHeight() - 60, "Health:", 24, 0x000000FF);




	currentBrickStruct.isEmpty = false;

}

LevelEditor::~LevelEditor()
{
	cursorBrick->isWaitingDestroy = true;
	brickSettingDisplay->isWaitingDestroy = true;

	optionsPanel->isWaitingDestroy = true;
	optionsPanelText->isWaitingDestroy = true;


	optionsPanelLevelText->isWaitingDestroy = true;
	levelNameInput->isWaitingDestroy = true;
	levelNameText->isWaitingDestroy = true;

	saveButton->isWaitingDestroy = true;
	overrideButton->isWaitingDestroy = true;
	loadButton->isWaitingDestroy = true;
	newButton->isWaitingDestroy = true;

	mainMenuButton->isWaitingDestroy = true;

	healthInputBox->isWaitingDestroy = true;
	healthText->isWaitingDestroy = true;

	redInputBoxBack->isWaitingDestroy = true;
	redTextBack->isWaitingDestroy = true;
	greenInputBoxBack->isWaitingDestroy = true;
	greenTextBack->isWaitingDestroy = true;
	blueInputBoxBack->isWaitingDestroy = true;
	blueTextBack->isWaitingDestroy = true;
	backgroundChooseText->isWaitingDestroy = true;

	redInputBoxBrick->isWaitingDestroy = true;
	redTextBrick->isWaitingDestroy = true;
	greenInputBoxBrick->isWaitingDestroy = true;
	greenTextBrick->isWaitingDestroy = true;
	blueInputBoxBrick->isWaitingDestroy = true;
	blueTextBrick->isWaitingDestroy = true;

	if (levelSelectMenu != nullptr) {
		levelSelectMenu->isWaitingDestroy = true;
	}


	for (int i = 0; i < 130; i++) {
		if (bricks[i] != nullptr) {
			bricks[i]->isWaitingDestroy = true;
		}
		
	}

}

void LevelEditor::Update(float DeltaTime)
{
	cooldown -= DeltaTime;
	if (!levelSelectOpen && cooldown <= 0) {
		Vector2 mp = GetMousePosition();
		Vector2 bp = { fminf(66 * ((int)(mp.x- Game::WorldBorders.x) / 66), 66 * 12), fminf(33 * ((int)(mp.y - Game::WorldBorders.y) / 33), 33 * 12) };
		
		cursorBrick->hasSprite = true;

		if (mp.y < Game::WorldBorders.y + (33*13) && mp.y >= Game::WorldBorders.y && mp.x < Game::WorldBorders.z && mp.x >= Game::WorldBorders.x) {
			cursorBrick->hasSprite = true;
			cursorBrick->physics->SetPosition(bp.x+ Game::WorldBorders.x, bp.y+ Game::WorldBorders.y);
			// Create new bricks
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
				int index = ((bp.y / 33) * 13) + (bp.x / 66);

				map.AddBrick(currentBrickStruct, index);

				// Reset display brick
				delete bricks[index];
				bricks[index] = nullptr;

				// Create new display brick
				bricks[index] = new UIObject();
				bricks[index]->physics->SetPosition(bp.x + Game::WorldBorders.x, bp.y + Game::WorldBorders.y);
				bricks[index]->CopySpriteByReference(cursorBrick->sprite);
				bricks[index]->sprite->colour = GetColor(currentBrickStruct.colour);
			}

			// Erase bricks
			if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
				cursorBrick->hasSprite = false;
				int index = ((bp.y / 33) * 13) + (bp.x / 66);

				map.AddBrick(BrickStruct(), index);

				// Reset display brick
				delete bricks[index];
				bricks[index] = nullptr;
			}

			// Copy colour from hovered brick
			if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
				int index = ((bp.y / 33) * 13) + (bp.x / 66);

				Color c = GetColor(map.mapStruct.bricks[index].colour);
				redInputBoxBrick->currentText = std::to_string(c.r);
				greenInputBoxBrick->currentText = std::to_string(c.g);
				blueInputBoxBrick->currentText = std::to_string(c.b);
			}


		}
		// Keep cursor dispaly brick on top
		cursorBrick->AddToGameWorld();
	}
	

	

	// Get settings values
	map.mapName = levelNameInput->boxText->text;


	currentBrickStruct.health = std::stoi(healthInputBox->boxText->text);

	Color col = Color();
	col.r = std::stoi(redInputBoxBrick->boxText->text);
	col.g = std::stoi(greenInputBoxBrick->boxText->text);
	col.b = std::stoi(blueInputBoxBrick->boxText->text);
	col.a = 255;
	currentBrickStruct.colour = ColorToInt(col);


	Color bCol = Color();
	bCol.r = std::stoi(redInputBoxBack->boxText->text);
	bCol.g = std::stoi(greenInputBoxBack->boxText->text);
	bCol.b = std::stoi(blueInputBoxBack->boxText->text);
	bCol.a = 255;
	map.mapStruct.backgroundColour = ColorToInt(bCol);

	cursorBrick->sprite->colour = col;
	brickSettingDisplay->sprite->colour = col;
}


void LevelEditor::SaveLevel()
{
	map.SaveMap();
	overrideButton->isActive = true;
}
void LevelEditor::OverrideLevel()
{
	map.SaveMap(map.loadedIndex);
}

void LevelEditor::NewLevel()
{
	for (int i = 0; i < 169; i++) {
		// Reset display brick
		delete bricks[i];
		bricks[i] = nullptr;

		map.mapStruct.bricks[i] = BrickStruct();
	}

	levelNameInput->currentText = "New Level";
	overrideButton->isActive = false;


}




void LevelEditor::ToggleLoadMenu() {
	levelSelectOpen = !levelSelectOpen;
	cooldown = 1;

	if (!levelSelectOpen) {
		levelSelectMenu->isWaitingDestroy = true;
		return;
	}

	levelSelectMenu = new LevelSelectMenu(GetScreenWidth()/2, GetScreenHeight()/2);
	levelSelectMenu->confirmButton->AssignCallMethod(std::bind(&LevelEditor::OpenLevel, this));
}

void LevelEditor::OpenLevel() {
	// If no level was chosen
	if (levelSelectMenu->chosenIndex == -1) {
		return;
	}
	ToggleLoadMenu();
	
	map.LoadMap(levelSelectMenu->chosenIndex);
	overrideButton->isActive = true;
	levelNameInput->currentText = map.GetMapName(levelSelectMenu->chosenIndex);
	

	Color bCol = GetColor(map.mapStruct.backgroundColour);
	redInputBoxBack->currentText = std::to_string(bCol.r);
	greenInputBoxBack->currentText = std::to_string(bCol.g);
	blueInputBoxBack->currentText = std::to_string(bCol.b);

	for (int i = 0; i < 13; i++) {
		for (int j = 0; j < 13; j++) {
			int index = (i * 13) + j;

			// Reset display brick
			delete bricks[index];
			bricks[index] = nullptr;


			if (map.mapStruct.bricks[index].isEmpty) {
				continue;
			}

			
			// Create new display brick
			UIObject* b = new UIObject();
			b->physics->SetPosition((j*66) + Game::WorldBorders.x, (i*33) + Game::WorldBorders.y);
			b->CopySpriteByReference(cursorBrick->sprite);
			b->sprite->colour = GetColor(map.mapStruct.bricks[index].colour);
			bricks[index] = b;
		}
	}
}