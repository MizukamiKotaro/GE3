#include "Punch.h"
#include <SoUtils/SoLib/SoLib.h>
#include <BlockManager.h>
#include <ModelDataManager.h>

Punch::Punch() {
}

void Punch::Init() {
	scale_ = Vector3::one;
	rotate_ = Vector3::zero;
	sphere_ = Sphere{ .center_ = Vector3::zero, .radius_ = 1.f };

	origin_ = Vector3::right;
	target_ = Vector3::zero;

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
		sphere_.center_ = SoLib::Lerp(origin_, target_, attackTimer_.GetProgress());

		if (attackTimer_.IsFinish()) {
			followTimer_.Start();
		}
	}
}

void Punch::Attack()
{
	if (attackTimer_.IsFinish() && followTimer_.IsFinish()) {
		attackTimer_.Start();
	}

}

bool Punch::IsAttacked() const
{
	return false;
}

float Punch::GetDamage() const
{
	return 0.0f;
}

void Punch::CalcTransMat() {

	transMat_ = Matrix4x4::MakeAffinMatrix(sphere_.radius_ * kModelScale_ * scale_, rotate_, sphere_.center_);
}
