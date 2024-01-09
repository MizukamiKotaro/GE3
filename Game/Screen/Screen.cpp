#include "Screen.h"
#include "Camera.h"
#include "TextureManager.h"
#include <numbers>

Screen::Screen()
{
	model_ = std::make_unique<Model>("plane");
	model_->SetTex(TextureManager::GetInstance()->LoadTexture("Resources/white.png"));
	model_->transform_.translate_ = { 0.0f,4.0f,4.0f };
	model_->transform_.scale_ = { 3.2f * 1.4f,1.8f * 1.4f,1.0f };
	model_->transform_.rotate_.y = std::numbers::pi_v<float>;
	model_->Update();
	model_->UnUsedLight();
	model_->SetColor(Vector4{ 0.0f,0.0f,0.0f,1.0f });
}

void Screen::Initialize() {
	
}

void Screen::Update() {

}

void Screen::Draw(Camera* camera) {
	model_->Draw(*camera);
}

void Screen::SetGPUHandle(D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle)
{
	model_->SetSRVGPUDescriptorHandle_(srvGPUDescriptorHandle);
	model_->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
}
