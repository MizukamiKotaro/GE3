#include "RotateState.h"
#include "../Boss.h"
#include "../Player.h"
#include "EscapeState.h"

void RotateState::Init()
{

	defaultScale_ = GetBoss()->GetTransform().scale_;

	stateArray_.push_back(AttackParameter{ .totalTime_ = 0.75f, .damage_ = false, .initFunc_ = &RotateState::StartUpInit, .updateFunc_ = &RotateState::StartUpUpdate });

	stateArray_.push_back(AttackParameter{ .totalTime_ = 3.f, .damage_ = true, .initFunc_ = &RotateState::RotateInit, .updateFunc_ = &RotateState::RotateUpdate });
	stateArray_.push_back(AttackParameter{ .totalTime_ = 1.f, .damage_ = false, .updateFunc_ = &RotateState::EndRotateUpdate });

	stateArray_.push_back(AttackParameter{ .initFunc_ = &RotateState::ChangeState });

	stateTimer_.Start(stateArray_[stateIndex_].totalTime_);
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

void RotateState::OnCollision(IEntity *other) {

	Player *player = dynamic_cast<Player *>(other);
	if (player) {
		Vector3 toPlayer = Vector3{ player->GetSphere().center_ - GetBoss()->GetTransform().translate_ }.Normalize();

		// ダメージがあるなら接触
		if (stateArray_[stateIndex_].damage_) {
			if (player->Damage(1.f)) {
				player->AddAcceleration(toPlayer * 5.f);
			}
		}
	}

}

bool RotateState::IsAttacked() const
{
	return stateArray_[stateIndex_].damage_;
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

void RotateState::StartUpInit() {
}

void RotateState::StartUpUpdate(const float deltaTime)
{
	Boss *boss = GetBoss();

	boss->GetTransform().scale_ = defaultScale_ * SoLib::Lerp(1.f, 0.75f, SoLib::easeInOutBack(stateTimer_.GetProgress()));

}

void RotateState::RotateInit() {
	scaleTimer_.Start(0.75f);
}

void RotateState::RotateUpdate(const float deltaTime)
{

	Boss *boss = GetBoss();
	auto &transform = boss->GetTransform();

	scaleTimer_.Update(deltaTime);
	if (scaleTimer_.IsFinish()) {
		scaleTimer_.Start();
		scaleCount_++;
	}
	if (scaleCount_ >= 3u) {
		NextParam();
	}

	transform.rotate_.z = SoLib::Lerp(0.f, SoLib::Math::Angle::PI2, SoLib::easeInOutSine(scaleTimer_.GetProgress()));


	boss->GetTransform().scale_ = defaultScale_ * SoLib::Lerp(0.75f, 1.25f, SoLib::easeInOutBack(stateTimer_.GetProgress()));


}

void RotateState::EndRotateUpdate(const float deltaTime) {

	Boss *boss = GetBoss();

	boss->GetTransform().scale_ = defaultScale_ * SoLib::Lerp(1.25f, 1.f, SoLib::easeInOutBack(stateTimer_.GetProgress()));

}
