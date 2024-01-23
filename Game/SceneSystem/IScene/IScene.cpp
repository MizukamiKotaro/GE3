#include "IScene.h"
#include "Ease/Ease.h"
#include "FrameInfo/FrameInfo.h"

// タイトルシーンで初期化
int IScene::sceneNo_ = TITLE;

int IScene::stageNo_ = 0;

IScene::~IScene() {}

void IScene::FirstInit()
{
	textureManager_ = TextureManager::GetInstance();
	modelDataManager_ = ModelDataManager::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();

	black_ = std::make_unique<Sprite>("Resources/white.png");
	black_->size_ = { 1280.0f,720.0f };
	black_->pos_ = { 640.0f,360.0f };
	black_->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
	black_->Update();

	backgroundSprite_ = std::make_unique<Sprite>("Resources/white.png");
	backgroundSprite_->size_ = { 1280.0f,720.0f };
	backgroundSprite_->pos_ = { 640.0f,360.0f };
	backgroundSprite_->SetColor(Vector4{ 0.3f,0.3f,0.3f,1.0f });
	backgroundSprite_->Update();
}

int IScene::GetSceneNo() { return sceneNo_; }

int IScene::GetStageNo() { return stageNo_; }

void IScene::FromBlackInitialize()
{
	if (sameScene_) {
		sameScene_ = false;
		Init();
	}

	transitionTimeCount_ = 0.0f;
	black_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void IScene::FromBlackUpdate()
{
	transitionTimeCount_ += FrameInfo::GetInstance()->GetDeltaTime();

	float alpha =
		Ease::UseEase(1.0f, 0.0f, transitionTimeCount_, kTransitionTime, Ease::EaseInSine, 2);
	black_->SetColor({ 0.0f, 0.0f, 0.0f, alpha });

	if (transitionTimeCount_ >= kTransitionTime) {
		black_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		transitionRequest_ = Transition::kOperation;
	}
}

void IScene::Play()
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
		Update();
		break;
	case Transition::kToBlack:
		ToBlackUpdate();
		break;
	default:
		break;
	}
}

void IScene::ToBlackInitialize()
{
	transitionTimeCount_ = 0.0f;
	black_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });
}

void IScene::ToBlackUpdate()
{
	transitionTimeCount_ += FrameInfo::GetInstance()->GetDeltaTime();

	float alpha =
		Ease::UseEase(0.0f, 1.0f, transitionTimeCount_, kTransitionTime, Ease::EaseInSine, 2);
	black_->SetColor({ 0.0f, 0.0f, 0.0f, alpha });

	if (transitionTimeCount_ >= kTransitionTime) {
		transitionRequest_ = Transition::kFromBlack;
		black_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		if (sceneNo_ == nextScene_) {
			sameScene_ = true;
		}
		else {
			sceneNo_ = nextScene_;
		}
	}
}

void IScene::ChangeScene(int sceneNo)
{
	nextScene_ = sceneNo;
	transitionRequest_ = Transition::kToBlack;
}
