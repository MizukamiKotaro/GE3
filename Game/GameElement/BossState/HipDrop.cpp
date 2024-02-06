#include "HipDrop.h"
#include "../Boss.h"
#include "../Player.h"
#include "EscapeState.h"

void HipDrop::Init() {
	stateArray_[0u] = AttackParameter{ .totalTime_ = 1.5f, .damage_ = false, .initFunc_ = &HipDrop::StartUpInit, .updateFunc_ = &HipDrop::StartUpUpdate };
	stateArray_[1u] = AttackParameter{ .totalTime_ = 10.f, .damage_ = false, .updateFunc_ = &HipDrop::FollowUpdate };
	stateArray_[2u] = AttackParameter{ .totalTime_ = 0.5f, .damage_ = true, .initFunc_ = &HipDrop::FallDownInit, .updateFunc_ = &HipDrop::FallDownUpdate };
	stateArray_[3u] = AttackParameter{ .totalTime_ = 2.f, .damage_ = false, };
	stateArray_[4u] = AttackParameter{ .initFunc_ = &HipDrop::ChangeState };

	stateTimer_.Start(stateArray_[stateIndex_].totalTime_);
	if (stateArray_[stateIndex_].initFunc_) {
		(this->*stateArray_[stateIndex_].initFunc_)();
	}

}

void HipDrop::Update(const float deltaTime) {
	GetBoss()->SetAcceleration({}); // 加速度の無効化
	GetBoss()->SetVelocity({}); // 速度の無効化

	stateTimer_.Update(deltaTime);

	if (stateTimer_.IsActive()) {

		// 関数が指定されているなら実行
		if (stateArray_[stateIndex_].updateFunc_) {
			(this->*stateArray_[stateIndex_].updateFunc_)(deltaTime);
		}
	}

	if (stateTimer_.IsFinish()) {

		NextParam();
	}

}

void HipDrop::ChangeState() {
	GetBoss()->ChangeState<EscapeState>();
}

void HipDrop::OnCollision(IEntity *other) {
	// プレイヤであった場合
	Player *player = dynamic_cast<Player *>(other);
	if (player) {
		Vector3 toPlayer = Vector3{ player->GetSphere().center_ - GetBoss()->GetTransform().translate_ }.Normalize();

		player->AddAcceleration(toPlayer * 7.5f);
	}
}

void HipDrop::StartUpInit() {
	beginPos_ = GetBoss()->GetTransform().translate_;

	endPos_ = beginPos_;
	endPos_.y = stageSize_.y;
}

void HipDrop::StartUpUpdate(const float deltaTime) {
	auto &transform = GetBoss()->GetTransform();

	// 時間を使ってアニメーション
	transform.translate_ = SoLib::Lerp(beginPos_, endPos_, stateTimer_.GetProgress());

}

void HipDrop::FollowUpdate(const float deltaTime) {

	auto &transform = GetBoss()->GetTransform();

	float diff = GetPlayer()->GetPos().x - transform.translate_.x;
	// 符号だけにする
	const float moveVec = diff / std::abs(diff);

	transform.translate_.x += moveVec * (MoveSpeed() * deltaTime);

	if (std::abs(diff) < 1.5f) {
		NextParam();
	}

}

void HipDrop::FallDownInit() {
	beginPos_ = GetBoss()->GetTransform().translate_;

	endPos_ = beginPos_;
	endPos_.y = -stageSize_.y;
}

void HipDrop::FallDownUpdate(const float deltaTime) {
	auto &transform = GetBoss()->GetTransform();

	// 時間を使ってアニメーション
	transform.translate_ = SoLib::Lerp(beginPos_, endPos_, stateTimer_.GetProgress());
}

void HipDrop::NextParam() {
	++stateIndex_;
	// インデックスが範囲内なら再度実行
	if (stateIndex_ < stateArray_.size()) {

		stateTimer_.Start(stateArray_[stateIndex_].totalTime_);

		// 関数が指定されているなら実行
		if (stateArray_[stateIndex_].initFunc_) {
			(this->*stateArray_[stateIndex_].initFunc_)();
		}
	}
}

bool HipDrop::IsAttacked() const {
	return stateArray_[stateIndex_].damage_;
}
