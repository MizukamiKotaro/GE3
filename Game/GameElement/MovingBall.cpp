#include "MovingBall.h"
#include <ModelDataManager.h>
#include "BlockManager.h"
#include "MapChip.h"
#include <SoUtils/SoLib/SoLib_Easing.h>

MapChip *MovingBall::mapChip_ = nullptr;

void MovingBall::Init() {

	model_ = ModelDataManager::GetInstance()->LoadObj("Sphere");
	sphere_.Initialize(Vector3::zero, 1.f);
	color_ = 0xFFFFFFFF;
	modelScale_ = 0.5f;

	aliveTime_.Start();

	color_ = 0x55FF55FF;

	velocity_ = {};
	acceleration_ = {};

	beforePos_ = sphere_.center_;
}

void MovingBall::Update([[maybe_unused]] const float deltaTime) {

	aliveTime_.Update(deltaTime);
	// 物理的に動かす
	UpdateRigidbody(deltaTime);

	acceleration_.y -= 9.8f * deltaTime * 2.f;

	sphere_.center_ = mapChip_->HitMap(beforePos_, sphere_.center_, 1.f);

	for (uint8_t i = 0u; i < 3u; i++) {
		if ((&beforePos_.x)[i] == (&sphere_.center_.x)[i]) {
			(&velocity_.x)[i] *= -0.9f;
		}
	}

	transformMat_ = Matrix4x4::MakeAffinMatrix(sphere_.scale_ * (sphere_.radius_ * modelScale_), sphere_.rotate_, sphere_.center_);

	color_.a = 1.f - SoLib::easeInCirc(aliveTime_.GetProgress());

	if (aliveTime_.IsFinish()) {
		isAlive_ = false;
	}

}

void MovingBall::Draw() {
	static  auto *const blockManager = BlockManager::GetInstance();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transformMat_, .color_ = color_ });

}

void MovingBall::AddAcceleration(const Vector3 &vec) {
	velocity_ = vec;
}

void MovingBall::SetMapChip(MapChip *mapChip) {
	mapChip_ = mapChip;
}

void MovingBall::UpdateRigidbody([[maybe_unused]] const float deltaTime) {
	beforePos_ = sphere_.center_;

	velocity_ += acceleration_;
	Vector3 fixVelocity = velocity_ * deltaTime;
	acceleration_ = {};

	sphere_.center_ += fixVelocity;
}
