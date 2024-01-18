#include "Sword.h"
#include <BlockManager.h>

Sword::Sword()
{
}

void Sword::Init()
{
	color_ = 0xFFFFFFFF;
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
