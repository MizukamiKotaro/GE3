#include "ClearScene.h"
#include "Kyoko.h"

ClearScene::ClearScene()
{
	FirstInit();

	skydome_ = std::make_unique<Skydome>();
	stage_ = std::make_unique<Stage>();
	screen_ = std::make_unique<Screen>();

	space_ = std::make_unique<Sprite>("Resources/space.png");
	space_->pos_ = { 630.0f,520.0f };
	space_->Update();

	s_ = std::make_unique<Sprite>("Resources/space.png");
	s_->pos_ = { 640.0f,600.0f };

	for (int i = 0; i < 4; i++) {

		if (i < 3) {
			if (i < 2) {
				comboNum_[i] = std::make_unique<Sprite>("Resources/scoreNum.png");
				comboNum_[i]->pos_ = { 700.0f + 64.0f * 2.0f + 64.0f * i, 160.0f };
				comboNum_[i]->size_ = { 64.0f, 64.0f };

				hitSp_[i] = std::make_unique<Sprite>("Resources/hit.png");
				hitSp_[i]->pos_ = { 400.0f, 80.0f * i + 160.0f };

				perNum_[i] = std::make_unique<Sprite>("Resources/scoreNum.png");
				perNum_[i]->pos_ = { 700.0f + 64.0f * 2.0f + 64.0f * i, 240.0f };
				perNum_[i]->size_ = { 64.0f, 64.0f };

				greNum_[i] = std::make_unique<Sprite>("Resources/scoreNum.png");
				greNum_[i]->pos_ = { 700.0f + 64.0f * 2.0f + 64.0f * i, 320.0f };
				greNum_[i]->size_ = { 64.0f, 64.0f };

				goodNum_[i] = std::make_unique<Sprite>("Resources/scoreNum.png");
				goodNum_[i]->pos_ = { 700.0f + 64.0f * 2.0f + 64.0f * i, 400.0f };
				goodNum_[i]->size_ = { 64.0f, 64.0f };

				missNum_[i] = std::make_unique<Sprite>("Resources/scoreNum.png");
				missNum_[i]->pos_ = { 700.0f + 64.0f * 2.0f + 64.0f * i, 480.0f };
				missNum_[i]->size_ = { 64.0f, 64.0f };
			}

			sp_[i] = std::make_unique<Sprite>("Resources/gameScene.png");
			sp_[i]->pos_ = { 400.0f,80.0f * i };
			sp_[i]->size_ = { 400.0f, 128.0f };
			sp_[i]->SetTextureTopLeft({ i * 200.0f, 0.0f });
			sp_[i]->SetTextureSize({ 200.0f, 64.0f });

			if (i == 2) {
				sp_[i]->SetTextureTopLeft({ 3 * 200.0f, 0.0f });
				sp_[i]->SetTextureSize({ 200.0f, 64.0f });
				sp_[i]->pos_ = { 640.0f, 600.0f };
			}

		}

		scoreNum_[i] = std::make_unique<Sprite>("Resources/scoreNum.png");
		scoreNum_[i]->pos_ = { 700.0f + 64.0f * i, 80.0f };
		scoreNum_[i]->size_ = { 64.0f, 64.0f };
	}

}

void ClearScene::Init()
{

}

void ClearScene::Update()
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
		ToBlackUpdate(TITLE);
		break;
	default:
		break;
	}
}

void ClearScene::Draw()
{
	Kyoko::PreDraw();

	skydome_->Draw(camera_.get());
	stage_->Draw(camera_.get());
	screen_->Draw(camera_.get());

	space_->Draw(*camera_.get());

	BlackDraw();

	Kyoko::PostDraw();
}


