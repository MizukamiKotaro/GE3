#include "Hole.h"
#include "MovingBall.h"
#include <BlockManager.h>
#include <ModelDataManager.h>
#include "Stage.h"
#include "MovingBall.h"

std::array<uint32_t, static_cast<uint32_t>(Hole::WeaponType::kMaxSize)> Hole::modelList_;
void Hole::StaticInit() {
	modelList_ = { ModelDataManager::GetInstance()->LoadObj("miniSword"),ModelDataManager::GetInstance()->LoadObj("miniHand"),ModelDataManager::GetInstance()->LoadObj("miniArrow") };
}
Hole::Hole() {
}

void Hole::Init() {
	playerCount_ = 0u;
	enemyCount_ = 0u;

	activeCount_ = 5u;

	position_ = {};
	rotate_ = Vector3{ 0.f,-90._deg,0.f };
	radius_ = 0.75f;

	model_ = ModelDataManager::GetInstance()->LoadObj("bumper");

	CalcTransMat();

}

void Hole::Update([[maybe_unused]] const float deltaTime) {


	UpdateBallChacher(deltaTime);

	CalcTransMat();

}

void Hole::Draw() {
	static auto *const blockManager = BlockManager::GetInstance();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transMat_, .color_ = color_ });
	blockManager->AddBox(modelList_[static_cast<uint32_t>(weaponType_)], IBlock{ .transformMat_ = transMat_, .color_ = color_ });
}

void Hole::OnCollision([[maybe_unused]] IEntity *other) {

	//MovingBall *ball = dynamic_cast<MovingBall *>(other);
	//if (ball) {

	//	ballList_.push_back(BallCatcher{ .ball_ = ball,.time_ = pStage_->GetHoleChathTime(), .begin_ = ball->GetNowPos() });

	//}

}

void Hole::SetWeapon(IWeapon *weapon) {
	if (weapon) {
		weapon_ = weapon;
		if (dynamic_cast<Sword *>(weapon)) { weaponType_ = WeaponType::kSword; }
		else if (dynamic_cast<Punch *>(weapon)) { weaponType_ = WeaponType::kPunch; }
		else if (dynamic_cast<Needle *>(weapon)) { weaponType_ = WeaponType::kNeedle; }

	}
}

void Hole::UpdateBallChacher(const float deltaTime)
{
	/*std::erase_if(ballList_, [this](Hole::BallCatcher &ball)->bool
		{
			if (ball.time_ <= 0.f) {

				ball.ball_->SetIsAlive(false);

				if (weapon_) {
					weapon_->Attack();
				}

				return true;
			}
			return false;
		}
	);

	for (auto &ball : ballList_) {
		ball.time_ -= deltaTime;
		const float t = 1.f - (ball.time_ / pStage_->GetHoleChathTime());
		ball.ball_->SetPos(SoLib::Lerp(ball.begin_, position_, t));
	}*/

}

void Hole::CalcTransMat() {

	transMat_ = Matrix4x4::MakeAffinMatrix(Vector3::one * radius_ * kModelScale_, rotate_, position_);
}
