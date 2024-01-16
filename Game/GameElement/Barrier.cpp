#include "Barrier.h"
#include "BlockManager.h"
#include "Player.h"
#include <ModelDataManager.h>
#include "MovingBall.h"
#include <calc.h>

void BarrierItem::Init() {
	attackTimer_.Start(0.5f);
	coolTimer_.Start(0.5f);

	color_ = 0xFF0000FF;
	//eulerRot_.y = 90._deg;

	model_ = ModelDataManager::GetInstance()->LoadObj("Barrier");

	attackSE_ = Audio::GetInstance()->LoadWave("Resources/playerSE/Attack.wav");
}

void BarrierItem::Update([[maybe_unused]] const float deltaTime) {
	attackTimer_.Update(deltaTime);
	coolTimer_.Update(deltaTime);

	color_.a = 1.f - attackTimer_.GetProgress();

	if (attackTimer_.IsActive()) {
		for (auto &ball : *ballList_) {
			const Vector3 diff = ball->GetNowPos() - *reinterpret_cast<Vector3 *>(transformMat_.m[3]);
			const auto stickDirection = Vector3{ direction_.x,direction_.y,0.f };

			if (Calc::MakeLength(diff) <= radius_ && Calc::Dot(stickDirection, diff) > barrierAngle_) {
				ball->AddAcceleration(stickDirection * 15.f);
				ball->SetTeam(MovingBall::Team::kPlayer);
			}
		}
	}
}

void BarrierItem::Draw() {
	static auto *const blockManager = BlockManager::GetInstance();

	eulerRot_.z = std::atan2(direction_.y, direction_.x);
	transformMat_ = Matrix4x4::MakeRotateMatrix(eulerRot_, Matrix4x4::RotateType::kXYZ);

	*reinterpret_cast<Vector3 *>(transformMat_.m[3]) += *reinterpret_cast<const Vector3 *>(parent_->GetTransMat().m[3]);

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transformMat_, .color_ = color_ });
}

void BarrierItem::Attack(const Vector2 direction) {

	if (coolTimer_.IsFinish()) {
		attackSE_.Play(false, 0.6f);

		coolTimer_.Start();
		attackTimer_.Start();
		direction_ = direction;


	}

}

void BarrierItem::SetBallList(std::list<std::unique_ptr<MovingBall>> *ballList) {
	ballList_ = ballList;
}
