#include "LevelEditor.h"

LevelEditor::LevelEditor()
{
	LoadSprite((char*)"TestBackground2.png");


}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::Update(float DeltaTime)
{
	GetMousePosition();
}
