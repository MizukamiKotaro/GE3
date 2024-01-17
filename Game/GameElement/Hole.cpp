#include "Hole.h"
#include "MovingBall.h"
#include <BlockManager.h>
#include <ModelDataManager.h>

Hole::Hole() {
}

void Hole::Init() {
	playerCount_ = 0u;
	enemyCount_ = 0u;

	activeCount_ = 5u;

	position_ = {};
	radius_ = 1.f;

	model_ = ModelDataManager::GetInstance()->LoadObj("Sphere");

	CalcTransMat();

}

void Hole::Update([[maybe_unused]] const float deltaTime) {

	CalcTransMat();

}

void Hole::Draw() {
	static auto *const blockManager = BlockManager::GetInstance();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transMat_, .color_ = color_ });
}

void Hole::OnCollision(IEntity *other) {

	MovingBall *ball = dynamic_cast<MovingBall *>(other);
	if (ball) {



	}

}

void Hole::CalcTransMat() {

	transMat_ = Matrix4x4::MakeAffinMatrix(Vector3::one * radius_ * kModelScale_, Vector3::zero, position_);
}
