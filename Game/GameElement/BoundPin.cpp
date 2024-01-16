#include "BoundPin.h"
#include <ModelDataManager.h>
#include <SoUtils/Math/Euler.h>
#include <BlockManager.h>

BoundPin::BoundPin() {
}

void BoundPin::Init() {

	model_ = ModelDataManager::GetInstance()->LoadObj("Sphere");

	color_ = 0xFFFF55FF;

	radius_ = 0.3f;

}

void BoundPin::Update(const float deltaTime) {

	CalcTransMat();
}

void BoundPin::Draw() {
	static auto *const blockManager = BlockManager::GetInstance();

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transMat_,.color_ = color_ });

}

void BoundPin::CalcTransMat() {
	transMat_ = Matrix4x4::MakeAffinMatrix(Vector3::one * radius_, Vector3::zero, position_);
}
