#include "BallSpawner.h"
#include "Stage.h"
#include <RandomGenerator/RandomGenerator.h>

Stage *BallSpawner::pStage_ = nullptr;

BallSpawner::BallSpawner() {
}

void BallSpawner::Init() {
	position_ = {};
}

void BallSpawner::Update(const float deltaTime) {

	fireSpanTimer_.Update(deltaTime);

	if (fireSpanTimer_.IsFinish()) {
		fireSpanTimer_.Start(fireSpan_);

		Fire();
	}

}

void BallSpawner::Draw() {

}

void BallSpawner::Fire() {

	pStage_->GetBallList()->push_back(std::make_unique<MovingBall>());
	auto ball = pStage_->GetBallList()->back().get();

	ball->Init();

	ball->SetPos(this->position_);
	const float cFireAngle = RandomGenerator::GetInstance()->RandFloat(-fireAngle_, fireAngle_);
	ball->SetVelocity(-Vector3::up * Matrix4x4::MakeRotateAxisAngle(Vector3::front, cFireAngle));

}
