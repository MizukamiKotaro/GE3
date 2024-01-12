#include "Player.h"
#include "calc.h"
#include <ModelDataManager.h>
#include "BlockManager.h"
#include "MovingBall.h"
#include "MapChip.h"
#include "Audio.h"

void Player::Init() {
	barrier_ = std::make_unique<BarrierItem>();
	barrier_->Init();
	barrier_->SetParent(this);

	model_ = ModelDataManager::GetInstance()->LoadObj("Sphere");
	color_ = 0xFFFFFFFF;

	scale_ = Vector3::one;

	audio_ = Audio::GetInstance();

	jumpSE_ = audio_->LoadWave("Resources/playerSE/jump.wav");
}

void Player::Update([[maybe_unused]] const float deltaTime) {
	barrier_->Update(deltaTime);

	transformMat_ = Matrix4x4::MakeAffinMatrix(scale_ * 0.5f, rotate_, transform_);

	acceleration_.y -= 9.8f * deltaTime * 2.f;

	//static const float kMaxAcceleration = 2.f;

	//for (uint8_t i = 0u; i < 3u; i++) {
	//	if (std::abs((&acceleration_.x)[i]) >= kMaxAcceleration) {
	//		(&acceleration_.x)[i] = std::clamp((&acceleration_.x)[i], -kMaxAcceleration, kMaxAcceleration);
	//	}
	//}

	// 移動処理を行う
	UpdateRigidbody(deltaTime);

	// 移動処理の更新


	transform_ = mapData_->HitMap(beforePos_, transform_, 1.f);

	if (beforePos_.y == transform_.y && velocity_.y < 0.f && not isLanding_) {
		Landing();
	}
	if (beforePos_.y != transform_.y) {
		isLanding_ = false;
	}

	for (uint8_t i = 0u; i < 3u; i++) {
		if ((&beforePos_.x)[i] == (&transform_.x)[i]) {
			(&velocity_.x)[i] = 0.f;
		}
	}

}

void Player::Draw() {
	static auto* const blockManager = BlockManager::GetInstance();

	barrier_->Draw();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transformMat_,.color_ = color_ });
}

void Player::InputAction(Input* const input, const float deltaTime) {
	Vector2 inputRight = input->GetGamePadRStick();

	if (Calc::MakeLength(inputRight)) {
		barrier_->Attack(Calc::Normalize(inputRight));
	
	}
	

	Vector2 inputLeft = input->GetGamePadLStick();
	if (inputLeft.x == 0.f) {
		inputLeft.x -= input->PressingKey(DIK_A);
		inputLeft.x += input->PressingKey(DIK_D);
	}
	if (inputLeft.x) {
		Move(inputLeft.x * 200.f, deltaTime);
	}

	if (isLanding_ && (input->PressedKey(DIK_SPACE) || input->PressedGamePadButton(Input::GamePadButton::A) || input->PressedGamePadButton(Input::GamePadButton::RIGHT_SHOULDER))) {
		acceleration_.y += 10.f;
		audio_->Play(jumpSE_, false, 0.6f);
	}

}

void Player::Move(const float power, [[maybe_unused]] const float deltaTime) {
	velocity_.x += power * deltaTime;
}



void Player::UpdateRigidbody([[maybe_unused]] const float deltaTime) {
	beforePos_ = transform_;

	velocity_ += acceleration_;
	acceleration_ = {};
	Vector3 fixVelocity = velocity_ * deltaTime;
	velocity_.x = 0.f;

	transform_ += fixVelocity;
}

void Player::SetMapChip(MapChip* const mapChip) {
	mapData_ = mapChip;
}

void Player::SetBallList(std::list<std::unique_ptr<MovingBall>>* ballList) {
	barrier_->SetBallList(ballList);
	ballList_ = ballList;
}

void Player::Landing() {
	isLanding_ = true;

	for (uint8_t i = 0u; i < 2u; i++) {
		ballList_->push_back(std::make_unique<MovingBall>());
		auto ball = ballList_->back().get();
		ball->Init();
		ball->SetPos(transform_ + Vector3{ .x = -1.f + 2.f * i });
		ball->SetVelocity(Vector3{ .y = 10.f });
	}
}