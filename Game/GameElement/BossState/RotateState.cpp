#include "RotateState.h"
#include "../Boss.h"
#include "../Player.h"
#include "EscapeState.h"

void RotateState::Init()
{
	stateArray_.push_back(AttackParameter{ .totalTime_ = 0.75f, .damage_ = false, .initFunc_ = &RotateState::StartUpInit, .updateFunc_ = &RotateState::StartUpUpdate });

	stateArray_.push_back(AttackParameter{ .totalTime_ = 3.f, .damage_ = true, .updateFunc_ = &RotateState::RotateUpdate });
	stateArray_.push_back(AttackParameter{ .totalTime_ = 1.f, .damage_ = false, });

	stateArray_.push_back(AttackParameter{ .initFunc_ = &RotateState::ChangeState });

}

void RotateState::Update(const float deltaTime)
{
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

void RotateState::OnCollision(IEntity *other)
{
}

bool RotateState::IsAttacked() const
{
	return false;
}

void RotateState::ChangeState() {
	GetBoss()->ChangeState<EscapeState>();
}

void RotateState::NextParam()
{
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

void RotateState::StartUpInit()
{
}

void RotateState::StartUpUpdate(const float deltaTime)
{
	Boss *boss = GetBoss();

	boss->GetTransform().scale_;

}

void RotateState::RotateUpdate(const float deltaTime)
{
}
