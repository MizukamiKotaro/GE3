#include "ClearPlayer.h"
#include "ImGuiManager/ImGuiManager.h"
#include "ModelDataManager.h"
#include <cmath>
#include "Camera.h"
#include "FrameInfo/FrameInfo.h"

ClearPlayer::ClearPlayer()
{
	hoges_[0] = std::make_unique<Model>("playerBody");
	hoges_[1] = std::make_unique<Model>("playerEyeL");
	hoges_[1]->transform_.parent_ = &hoges_[0]->transform_;
	hoges_[2] = std::make_unique<Model>("playerEyeR");
	hoges_[2]->transform_.parent_ = &hoges_[0]->transform_;
	hoges_[3] = std::make_unique<Model>("playerMouth");
	hoges_[3]->transform_.parent_ = &hoges_[0]->transform_;

	hoges_[0]->transform_.translate_ = { -7.9f,-5.0f,-5.0f };
	hoges_[0]->transform_.scale_ *= 1.5f;
	hoges_[0]->transform_.rotate_.y = -1.7f;

	for (int i = 0; i < 4; i++) {
		hoges_[i]->Update();
	}
	vel_ = 0.0f;

	time_ = 0.0f;
}

void ClearPlayer::Initialize()
{
	hoges_[0]->transform_.translate_ = { 0.3f, -10.3f, -20.9f };
	hoges_[0]->transform_.rotate_ = { 0.0f,-0.95f,0.1f };

	for (int i = 0; i < 4; i++) {
		hoges_[i]->Update();
	}

	vel_ = 0.0f;

	time_ = 0.0f;
}

void ClearPlayer::Update()
{
	vel_ -= 0.04f;

	hoges_[0]->transform_.translate_.y += vel_;

	if (hoges_[0]->transform_.translate_.y <= -5.0f) {
		hoges_[0]->transform_.translate_.y = -5.0f;
		vel_ = 0.7f;
	}

	for (int i = 0; i < 4; i++) {
		hoges_[i]->Update();
	}
}

void ClearPlayer::oUpdate()
{

	time_ += FrameInfo::GetInstance()->GetDeltaTime();



	for (int i = 0; i < 4; i++) {
		hoges_[i]->Update();
	}
}

void ClearPlayer::Draw(Camera* camera)
{
	for (int i = 0; i < 4; i++) {
		hoges_[i]->Draw(*camera);
	}
}
