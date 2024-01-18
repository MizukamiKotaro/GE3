#include "Sword.h"
#include <BlockManager.h>
#include "Stage.h"
#include "ModelDataManager.h"

Sword::Sword()
{
}

void Sword::Init()
{

	model_ = ModelDataManager::GetInstance()->LoadObj("sword");

	color_ = 0xFFFFFFFF;

	scale_ = Vector3::one * 5;
	rotate_ = Vector3::zero;
	position_ = Vector3::zero;

	rotate_.y = -90._deg;

}

void Sword::Update([[maybe_unused]] const float deltaTime) {
}

void Sword::Draw()
{
	static BlockManager *const blockManager = BlockManager::GetInstance();

	CalcTransMat();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transMat_, .color_ = color_ });

}

void Sword::CalcTransMat() {
	transMat_ = Matrix4x4::MakeAffinMatrix(scale_, rotate_, position_);
}
