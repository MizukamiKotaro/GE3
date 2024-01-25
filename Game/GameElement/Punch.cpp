#include "Punch.h"
#include <SoUtils/SoLib/SoLib.h>
#include <BlockManager.h>
#include <ModelDataManager.h>
#include "Stage.h"

Punch::Punch() {
}

void Punch::Init() {
	scale_ = Vector3::one * 4.f;
	rotate_ = Vector3{ 0._deg, -90._deg, 0._deg };
	translate_ = Vector3(13, -7, -3);
	//Vector3::right * 5.f;
	sphere_ = Sphere{ .radius_ = scale_.x * 0.5f };

	origin_ = Vector3(13, -7, -3);
	target_ = Vector3(-10, -7, -3);

	SetHandType(HandType::kRight);

	CalcTransMat();

}

void Punch::SetHandType(HandType type) {

	handType_ = type;
	switch (type) {
	case Punch::HandType::kLeft:
		model_ = ModelDataManager::GetInstance()->LoadObj("handLeft");
		break;
	case Punch::HandType::kRight:
		model_ = ModelDataManager::GetInstance()->LoadObj("handRight");
		break;
	default:
		break;
	}

}

void Punch::Update(const float deltaTime) {
	AttackUpdate(deltaTime);

	CalcTransMat();
}

void Punch::Draw()
{
	static BlockManager *const blockManager = BlockManager::GetInstance();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transMat_ });
}

void Punch::AttackUpdate(const float deltaTime) {

	attackTimer_.Update(deltaTime);
	followTimer_.Update(deltaTime);

	if (attackTimer_.IsActive()) {
		translate_ = SoLib::Lerp(origin_, target_, pStage_->GetPunchEase()(attackTimer_.GetProgress()));

		if (attackTimer_.IsFinish()) {
			followTimer_.Start(1.f);
		}
	}
	if (followTimer_.IsActive()) {
		translate_ = SoLib::Lerp(target_, origin_, pStage_->GetPunchEase()(followTimer_.GetProgress()));
	}

	CalcCollision();
}

void Punch::Attack(const AttackType attackType)
{
	if (not attackTimer_.IsActive() && not followTimer_.IsActive()) {
		SetIsHitBoss(false);
		attackTimer_.Start(1.f);
	}

}

bool Punch::IsAttacked() const
{
	return attackTimer_.IsActive();
}

float Punch::GetDamage() const
{
	return 1.0f;
}

const Sphere *Punch::GetCollision() const {
	// タイマーが止まってたら攻撃判定は無い
	if (not attackTimer_.IsActive()) { return nullptr; }

	return &sphere_;
}

void Punch::CalcCollision() {
	sphere_.center_ = translate_;
	sphere_.center_.z = 0.f;
	//sphere_.center_.y = -7.f;
}

void Punch::CalcTransMat() {

	transMat_ = Matrix4x4::MakeAffinMatrix(kModelScale_ * scale_, rotate_, translate_);
}
