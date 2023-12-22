#include "IScene.h"

// タイトルシーンで初期化
int IScene::sceneNo_ = TITLE;

int IScene::stagrNo_ = 0;

IScene::~IScene(){}

void IScene::FirstInit()
{
	textureManager_ = TextureManager::GetInstance();
	modelDataManager_ = ModelDataManager::GetInstance();
	audio_ = Audio::GetInstance();
	input_ = Input::GetInstance();

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
}

int IScene::GetSceneNo() { return sceneNo_; }

int IScene::GetStageNo() { return stagrNo_; }
