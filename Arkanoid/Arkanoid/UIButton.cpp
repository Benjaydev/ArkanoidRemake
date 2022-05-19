#include "UIButton.h"

UIButton::UIButton(float x, float y, float width, float height, int colour, int hColour, UIText* text)
{

	physics->SetPosition(x, y);
	

	buttonSprite = new UIPanel(x, y, width, height, colour);
	defaultColour = colour;
	hoverColour = hColour;
	Color d = GetColor(defaultColour);
	d.a = 100;
	deactiveColour = ColorToInt(d);



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

void UIButton::Update(float DeltaTime) {

	Vector2 offset = buttonText->GetCentreOffset();
	buttonText->physics->SetPosition(offset.x, offset.y);

	if (!isActive) {
		buttonSprite->sprite->colour = GetColor(deactiveColour);
		return;
	}


	buttonSprite->sprite->colour = GetColor(defaultColour);
	if (physics->collider->Overlaps(GetMousePosition())) {

		OnHover();
		// Button Click
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			OnClick();
		}
	}


	
}

void UIButton::OnHover()
{
	buttonSprite->sprite->colour = GetColor(hoverColour);
}

void UIButton::OnClick()
{
	if (callFunction != nullptr) {
		callFunction();
	}
	
}


