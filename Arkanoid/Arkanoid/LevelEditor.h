#pragma once
#include "UIObject.h"
class LevelEditor : public UIObject
{
public:
	LevelEditor();
	~LevelEditor();

	void Update(float DeltaTime) override;
};

