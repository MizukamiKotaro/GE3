#include "Player.h"
#include "calc.h"
#include <ModelDataManager.h>

void Player::Init() {
	barrier_ = std::make_unique<BarrierItem>();
	barrier_->Init();
	barrier_->SetParent(this);

	model_ = ModelDataManager::GetInstance()->LoadObj("Sphere");
}

void Player::Update([[maybe_unused]] const float deltaTime) {
	barrier_->Update(deltaTime);

	transformMat_ = Matrix4x4::MakeAffinMatrix(scale_, rotate_, transform_);

	UpdateRigidbody(deltaTime);
}

void Player::Draw() {
	barrier_->Draw();
}

void Player::InputAction(Input *const input, const float deltaTime) {
	Vector2 inputRight = input->GetGamePadRStick();


	if (Calc::MakeLength(inputRight)) {
		barrier_->Attack(Calc::Normalize(inputRight));
	}

	Vector2 inputLeft = input->GetGamePadLStick();
	if (inputLeft.x) {
		Move(inputLeft.x, deltaTime);
	}

}

void Player::Move(const float power, [[maybe_unused]] const float deltaTime) {
	acceleration_.x += power * deltaTime;
}



void Player::UpdateRigidbody([[maybe_unused]] const float deltaTime) {
	beforePos_ = transform_;

	velocity_ += acceleration_;
	acceleration_ = {};
	Vector3 fixVelocity = velocity_ * deltaTime;

	transform_ += fixVelocity;
}
