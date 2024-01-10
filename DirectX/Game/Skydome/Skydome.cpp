#include "Skydome.h"
#include "Camera.h"

Skydome::Skydome()
{
	model_ = std::make_unique<Model>("skydome");
	model_->Update();
	model_->UnUsedLight();
	model_->SetColor(Vector4{ 0.02f,0.02f,0.02f,1.0f });
}

void Skydome::Initialize() {
	
}

void Skydome::Update() {

}

void Skydome::Draw(Camera* camera) { 
	model_->Draw(*camera);
}
