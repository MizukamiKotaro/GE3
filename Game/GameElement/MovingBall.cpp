#include "MovingBall.h"
#include <ModelDataManager.h>
#include "BlockManager.h"

void MovingBall::Init() {

	model_ = ModelDataManager::GetInstance()->LoadObj("Sphere");
	sphere_.Initialize(Vector3::zero, 1.f);
	color_ = 0xFFFFFFFF;
	modelScale_ = 0.5f;

}

void MovingBall::Update([[maybe_unused]] const float deltaTime) {

	// 物理的に動かす
	//UpdateRigidbody(deltaTime);

	transformMat_ = Matrix4x4::MakeAffinMatrix(sphere_.scale_ * (sphere_.radius_ * modelScale_), sphere_.rotate_, sphere_.center_);


}

void MovingBall::Draw() {
	static  auto *const blockManager = BlockManager::GetInstance();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transformMat_, .color_ = color_ });

}

void MovingBall::UpdateRigidbody([[maybe_unused]] const float deltaTime) {
	beforePos_ = sphere_.center_;

	velocity_ += acceleration_;
	Vector3 fixVelocity = velocity_ * deltaTime;

	sphere_.center_ += fixVelocity;
}
