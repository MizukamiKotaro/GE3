#include "Needle.h"
#include <BlockManager.h>
#include <ModelDataManager.h>
#include "Stage.h"

Needle::Needle()
{
}

void Needle::Init()
{
	scale_ = Vector3::one;
	rotate_ = Vector3::zero;
	translate_ = Vector3::zero;
	rotate_.y = 90._deg;

	capsule_ = Capsule{ .radius_ = 1.f, .segment_{ Vector3::zero, Vector3::up } };

	CalcTransMat();

	model_ = ModelDataManager::GetInstance()->LoadObj("arrow");
}

void Needle::SetParametor() {

	scale_ = Vector3::one * pStage_->GetNeedleScale();
	translate_.y = pStage_->stageGround_;

	capsule_.radius_ = scale_.x * 0.2f;

	CalcSegment();

}

void Needle::Update(const float deltaTime)
{
	AttackUpdate(deltaTime);


	CalcTransMat();
}

void Needle::Draw()
{
	static BlockManager *const blockManager = BlockManager::GetInstance();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transMat_ });
}

void Needle::AttackUpdate(const float deltaTime)
{
	attackTimer_.Update(deltaTime);
	followTimer_.Update(deltaTime);

	if (attackTimer_.IsActive()) {
		translate_.y = SoLib::Lerp(pStage_->stageGround_, pStage_->GetNeedleHight(), pStage_->GetPunchEase()(attackTimer_.GetProgress()));

		if (attackTimer_.IsFinish()) {
			followTimer_.Start(1.f);
		}
	}
	if (followTimer_.IsActive()) {
		translate_.y = SoLib::Lerp(pStage_->GetNeedleHight(), pStage_->stageGround_, pStage_->GetPunchEase()(followTimer_.GetProgress()));
	}

	CalcSegment();

}

void Needle::Attack(const AttackType attackType)
{
	if (not attackTimer_.IsActive() && not followTimer_.IsActive()) {
		attackTimer_.Start(pStage_->GetNeedleTime());
	}

}

bool Needle::IsAttacked() const
{
	return attackTimer_.IsActive();
}

float Needle::GetDamage() const
{
	return 1.f;
}

const Capsule *Needle::GetCollision() const {
	if (not IsAttacked()) { return nullptr; }
	return &capsule_;
}

void Needle::CalcSegment() {
	capsule_.segment_.diff.y = scale_.y;
	capsule_.segment_.origin = translate_ + capsule_.segment_.diff * -0.5f;
	capsule_.segment_.origin.z = 0.f;
}

void Needle::CalcTransMat() {

	transMat_ = Matrix4x4::MakeAffinMatrix(scale_, rotate_, translate_);

}
