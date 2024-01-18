#include "Hole.h"
#include "MovingBall.h"
#include <BlockManager.h>
#include <ModelDataManager.h>
#include "Stage.h"
#include "MovingBall.h"

Stage *Hole::pStage_;

Hole::Hole() {
}

void Hole::Init() {
	playerCount_ = 0u;
	enemyCount_ = 0u;

	activeCount_ = 5u;

	position_ = {};
	radius_ = 0.75f;

	model_ = ModelDataManager::GetInstance()->LoadObj("Sphere");

	CalcTransMat();

}

void Hole::SetStage(Stage *stage)
{
	pStage_ = stage;
}

void Hole::Update([[maybe_unused]] const float deltaTime) {


	UpdateBallChacher(deltaTime);

	CalcTransMat();

}

void Hole::Draw() {
	static auto *const blockManager = BlockManager::GetInstance();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transMat_, .color_ = color_ });
}

void Hole::OnCollision(IEntity *other) {

	MovingBall *ball = dynamic_cast<MovingBall *>(other);
	if (ball) {

		ballList_.push_back(BallCatcher{ .ball_ = ball,.time_ = pStage_->GetHoleChathTime(), .begin_ = ball->GetNowPos() });

	}

}

void Hole::UpdateBallChacher(const float deltaTime)
{
	std::erase_if(ballList_, [](Hole::BallCatcher &ball)->bool
		{
			if (ball.time_ <= 0.f) {

				ball.ball_->SetIsAlive(false);
				return true;
			}
			return false;
		}
	);

	for (auto &ball : ballList_) {
		ball.time_ -= deltaTime;
		const float t = 1.f - (ball.time_ / pStage_->GetHoleChathTime());
		ball.ball_->SetPos(SoLib::Lerp(ball.begin_, position_, t));
	}

}

void Hole::CalcTransMat() {

	transMat_ = Matrix4x4::MakeAffinMatrix(Vector3::one * radius_ * kModelScale_, Vector3::zero, position_);
}
