#include "SelectScene.h"
#include "Kyoko.h"
#include "Notes/Notes.h"

SelectScene::SelectScene()
{
	FirstInit();

	skydome_ = std::make_unique<Skydome>();
	stage_ = std::make_unique<Stage>();
	screen_ = std::make_unique<Screen>();

	musicSprites_[SHINING_STAR] = std::make_unique<Sprite>("Resources/shining_star.png");
	musicSprites_[SHINING_STAR]->pos_ = { 640.0f,300.0f };
	musicSprites_[SHINING_STAR]->Update();
}

void SelectScene::Init()
{

}

void SelectScene::Update()
{
	if (transitionRequest_) {
		transition_ = transitionRequest_.value();

		switch (transition_)
		{
		case Transition::kFromBlack:
			FromBlackInitialize();
			break;
		case Transition::kOperation:
			break;
		case Transition::kToBlack:
			ToBlackInitialize();
			break;
		default:
			break;
		}

		transitionRequest_ = std::nullopt;
	}

	switch (transition_)
	{
	case Transition::kFromBlack:
		FromBlackUpdate();
		break;
	case Transition::kOperation:
		if (input_->PressedKey(DIK_SPACE)) {
			transitionRequest_ = Transition::kToBlack;
		}
		break;
	case Transition::kToBlack:
		ToBlackUpdate(STAGE);
		break;
	default:
		break;
	}
}

void SelectScene::Draw()
{
	Kyoko::PreDraw();

	skydome_->Draw(camera_.get());
	stage_->Draw(camera_.get());
	screen_->Draw(camera_.get());

	musicSprites_[SHINING_STAR]->Draw(*camera_.get());

	BlackDraw();

	Kyoko::PostDraw();
}

