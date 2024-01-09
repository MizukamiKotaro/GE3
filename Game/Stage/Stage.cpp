#include "Stage.h"
#include "Camera.h"
#include "TextureManager.h"

Stage::Stage()
{
	model_ = std::make_unique<Model>("ground");
	model_->SetTex(TextureManager::GetInstance()->LoadTexture("Resources/white.png"));
	model_->Update();
	model_->UnUsedLight();
	model_->SetColor(Vector4{ 0.1f,0.1f,0.1f,1.0f });
}

void Stage::Initialize() {
	
}

void Stage::Update() {

}

void Stage::Draw(Camera* camera) {
	model_->Draw(*camera);
}
