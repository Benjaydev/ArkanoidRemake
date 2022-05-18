#include "LevelEditor.h"
LevelEditor::LevelEditor()
{
	cursorBrick = new UIObject();
	LoadSprite((char*)"TestBackground2.png");
	sprite->colour = GetColor((int)0xFFFFFF66);
	cursorBrick->LoadSprite((char*)"Brick.png");

	
	optionsPanel = new UIPanel(0, GetScreenHeight()-300, GetScreenWidth(), 300, 0xFFFFFFAA);
	optionsPanelText = new UIText(GetScreenWidth()-350, GetScreenHeight() - 300, "Brick Settings", 48, 0x000000FF);
	optionsPanelLevelText = new UIText(10, GetScreenHeight() - 300, "Level Settings", 48, 0x000000FF);

	levelNameInput = new UIInputBox(125, GetScreenHeight() - 200, 200, 40, 0x585858FF, ColorToInt(BLUE), 0x8888FFFF, new UIText(0, 0, "New Level", 16, 0xFFFFFFFF), 20);
	levelNameText = new UIText(25, GetScreenHeight() - 250, "Level Name", 24, 0x000000FF);

	saveButton = new UIButton(75, GetScreenHeight() - 50, 100, 40, 0x585858FF, ColorToInt(BLUE), new UIText(0, 0, "Save", 16, 0xFFFFFFFF));
	saveButton->AssignCallMethod(std::bind(&LevelEditor::SaveLevel, this));





	// Brick colour choose
	Vector2 brickCCPos = { GetScreenWidth() - 200,  GetScreenHeight() - 225 };
	// Red
	redInputBoxBrick = new UIInputBox(brickCCPos.x, brickCCPos.y, 80, 40, 0x585858FF, ColorToInt(RED), 0xFF8888FF, new UIText(0, 0, "255", 16, 0xFFFFFFFF), 3, true);
	redInputBoxBrick->intMax = 255;
	redInputBoxBrick->intMin = 0;
	redTextBrick = new UIText(brickCCPos.x-100,brickCCPos.y-10, "Red", 24, 0x000000FF);

	// Green
	greenInputBoxBrick = new UIInputBox(brickCCPos.x, brickCCPos.y+50, 80, 40, 0x585858FF, ColorToInt(GREEN), 0x88FF88FF, new UIText(0, 0, "255", 16, 0xFFFFFFFF), 3, true);
	greenInputBoxBrick->intMax = 255;
	greenInputBoxBrick->intMin = 0;
	greenTextBrick = new UIText(brickCCPos.x-120, brickCCPos.y-10+50, "Green", 24, 0x000000FF);

	// Blue
	blueInputBoxBrick = new UIInputBox(brickCCPos.x, brickCCPos.y+100, 80, 40, 0x585858FF, ColorToInt(BLUE), 0x8888FFFF, new UIText(0, 0, "255", 16, 0xFFFFFFFF), 3, true);
	blueInputBoxBrick->intMax = 255;
	blueInputBoxBrick->intMin = 0;
	blueTextBrick = new UIText(brickCCPos.x-100, brickCCPos.y-10+100, "Blue", 24, 0x000000FF);

	brickSettingDisplay = new UIObject();
	brickSettingDisplay->LoadSprite((char*)"Brick.png");
	brickSettingDisplay->physics->SetPosition(brickCCPos.x + 100, brickCCPos.y - 10 + 50);
	brickSettingDisplay->AddToGameWorld();



	// Brick health input
	healthInputBox = new UIInputBox(GetScreenWidth()-100, GetScreenHeight() - 50, 40, 40, 0x585858FF, ColorToInt(BLUE), 0x8888FFFF, new UIText(0, 0, "1", 16, 0xFFFFFFFF), 1, true);
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

	healthInputBox->isWaitingDestroy = true;
	healthText->isWaitingDestroy = true;

	redInputBoxBrick->isWaitingDestroy = true;
	redTextBrick->isWaitingDestroy = true;

	greenInputBoxBrick->isWaitingDestroy = true;
	greenTextBrick->isWaitingDestroy = true;

	blueInputBoxBrick->isWaitingDestroy = true;
	blueTextBrick->isWaitingDestroy = true;

	for (int i = 0; i < 130; i++) {
		if (bricks[i] != nullptr) {
			bricks[i]->isWaitingDestroy = true;
		}
		
	}

}

void LevelEditor::Update(float DeltaTime)
{
	Vector2 mp = GetMousePosition();
	Vector2 bp = { fminf(66 * ((int)mp.x / 66), 66 * 12), fminf(33 * ((int)mp.y / 33), 33 * 9) };
	cursorBrick->physics->SetPosition(bp.x, bp.y);

	//std::cout << ((bp.y / 33) * 13) + (bp.x / 66) << std::endl;

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		int index = ((bp.y / 33) * 13) + (bp.x / 66);
		if (mp.y < 33 * 10 && mp.y >= 0 && mp.x < 66 * 13 && mp.x >= 0) {
			map.AddBrick(currentBrickStruct, index);

			// Reset display brick
			delete bricks[index];
			bricks[index] = nullptr;

			// Create new dispaly brick
			UIObject* b = new UIObject();
			b->physics->SetPosition(bp.x, bp.y);
			b->CopySpriteByReference(cursorBrick->sprite);
			b->sprite->colour = GetColor(currentBrickStruct.colour);
			bricks[index] = b;
		}
	}
	// Keep cursor dispaly brick on top
	cursorBrick->AddToGameWorld();

	// Get settings values
	map.mapName = levelNameInput->boxText->text;


	currentBrickStruct.health = std::stoi(healthInputBox->boxText->text);

	Color col = Color();
	col.r = std::stoi(redInputBoxBrick->boxText->text);
	col.g = std::stoi(greenInputBoxBrick->boxText->text);
	col.b = std::stoi(blueInputBoxBrick->boxText->text);
	col.a = 255;
	currentBrickStruct.colour = ColorToInt(col);

	cursorBrick->sprite->colour = col;
	brickSettingDisplay->sprite->colour = col;
}

void LevelEditor::SaveLevel()
{
	if (saveButton->buttonText->text == "Override") {
		map.SaveMap(map.loadedIndex);
	}

	map.SaveMap();
	saveButton->buttonText->text = "Override";
}
