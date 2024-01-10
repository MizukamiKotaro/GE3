#include "TitleScene.h"
#include "Kyoko.h"

TitleScene::TitleScene()
{
	FirstInit();

	skydome_ = std::make_unique<Skydome>();
	stage_ = std::make_unique<Stage>();
	screen_ = std::make_unique<Screen>();

	title_ = std::make_unique<Sprite>("Resources/title.png");
	title_->pos_ = { 630.0f,240.0f };
	title_->Update();

	space_ = std::make_unique<Sprite>("Resources/space.png");
	space_->pos_ = { 630.0f,520.0f };
	space_->Update();
}

void TitleScene::Init()
{
	space_->pos_ = { 640.0f,560.0f };
	space_->Update();
}

void TitleScene::Update()
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
		ToBlackUpdate(SELECT);
		break;
	default:
		break;
	}
}

void TitleScene::Draw()
{
	Kyoko::PreDraw();

	skydome_->Draw(camera_.get());
	stage_->Draw(camera_.get());
	screen_->Draw(camera_.get());

	title_->Draw(*camera_.get());

	space_->Draw(*camera_.get());

	BlackDraw();

	Kyoko::PostDraw();
}

