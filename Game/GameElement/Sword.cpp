#include "Sword.h"
#include <BlockManager.h>
#include "Stage.h"
#include "ModelDataManager.h"

Sword::Sword() {
}

void Sword::Init()
{

	model_ = ModelDataManager::GetInstance()->LoadObj("sword");

	color_ = 0xFFFFFFFF;

	scale_ = Vector3::one * 5.f;
	rotate_ = Vector3::zero;
	position_ = Vector3::zero;

	rotate_.y = -90._deg;

	capsule_.segment_.origin = position_;
	capsule_.segment_.diff = Vector3::up * 5.f;
	capsule_.radius_ = 2.f;

}

void Sword::Update([[maybe_unused]] const float deltaTime) {

	AttackUpdate(deltaTime);
}

void Sword::AttackUpdate(const float deltaTime) {

	attackTimer_.Update(deltaTime);

	beforeRotate_ = rotate_.z;

	rotate_.z = targetAngle_ * pStage_->GetSwordEase()(attackTimer_.GetProgress());

	capsule_.segment_.diff = Vector3::up * 5.f * Matrix4x4::MakeRotateAxisAngle(Vector3::front, rotate_.z);

}

void Sword::Draw()
{
	static BlockManager *const blockManager = BlockManager::GetInstance();

	CalcTransMat();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transMat_, .color_ = color_ });

}

void Sword::Attack(const AttackType attackType) {
	if (attackTimer_.IsFinish()) {
		attackTimer_.Start(pStage_->GetSwingTime());
	}
}

const Capsule *Sword::GetCollision() const {
	// タイマーが止まってたら攻撃判定は無い
	if (not attackTimer_.IsActive()) { return nullptr; }

	return &capsule_;
}

float Sword::GetDamage() const {
	return pStage_->GetSwordDamage();
}

void Sword::CalcTransMat() {
	transMat_ = Matrix4x4::MakeAffinMatrix(scale_, rotate_, position_);
}
