#include "Tackle.h"

#include "../Boss.h"
#include "../Player.h"
#include "EscapeState.h"

void TackleState::Init() {
	stateArray_[0u] = AttackParameter{ .totalTime_ = 1.f, .damage_ = 0.f, .updateFunc_ = &TackleState::RotateUpdate };
	stateArray_[1u] = AttackParameter{ .totalTime_ = 1.5f, .damage_ = 5.f, .initFunc_ = &TackleState::AttackStart,.updateFunc_ = &TackleState::AttackUpdate };
	stateArray_[2u] = AttackParameter{ .totalTime_ = 2.f, .damage_ = 0.f };
	stateArray_[3u] = AttackParameter{ .totalTime_ = 2.f, .damage_ = 0.f, .initFunc_ = &TackleState::ChangeState };

	stateTimer_.Start(stateArray_[stateIndex_].totalTime_);

}

void TackleState::Update(const float deltaTime) {

	stateTimer_.Update(deltaTime);

	if (stateTimer_.IsActive()) {

		// 関数が指定されているなら実行
		if (stateArray_[stateIndex_].updateFunc_) {
			(this->*stateArray_[stateIndex_].updateFunc_)();
		}
	}

	if (stateTimer_.IsFinish()) {

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

}

void TackleState::ChangeState() {
	GetBoss()->ChangeState<EscapeState>();
}

void TackleState::RotateUpdate() {
	auto &transform = GetBoss()->GetTransform();
	transform.rotate_.z = SoLib::easeInOutBack(stateTimer_.GetProgress()) * SoLib::Math::Angle::PI2;
}

void TackleState::AttackStart() {
	auto &transform = GetBoss()->GetTransform();
	attackVec_.origin = transform.translate_;

	Vector3 diff = GetPlayer()->GetGrobalPos() - transform.translate_;

	attackVec_.diff = diff.Normalize() * 7.5f;

}

void TackleState::AttackUpdate() {
	GetBoss()->GetTransform().translate_ = attackVec_.GetPoint(SoLib::easeInOutBack(stateTimer_.GetProgress()));

}
