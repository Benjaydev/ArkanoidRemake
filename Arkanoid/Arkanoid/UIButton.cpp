#include "UIButton.h"

UIButton::UIButton(float x, float y, float width, float height, int colour, UIText* text)
{

	physics->SetPosition(x, y);
	

	buttonSprite = new UIPanel(x, y, width, height, colour);
	AddChild(buttonSprite);
	Vector2 offset = buttonSprite->sprite->GetCentreOffset();
	buttonSprite->physics->SetPosition(offset.x, offset.y);

	physics->SetCollider(cType::Rectangle);
	physics->FitColliderWH(buttonSprite->sprite->GetWidth(), buttonSprite->sprite->GetHeight(), {x,y});

	buttonText = text;

	Vector2 textOffset = text->GetCentreOffset();
	AddChild(buttonText);
	text->physics->SetPosition(textOffset.x, textOffset.y);
	buttonText->AddToGameWorld();

	AddToGameWorld();
}

UIButton::~UIButton()
{
	buttonSprite->isWaitingDestroy = true;
	buttonText->isWaitingDestroy = true;

}


void UIButton::OnClick()
{
	
	callFunction();
}


