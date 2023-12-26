#include "Barrier.h"
#include "BlockManager.h"
#include "Player.h"
#include <ModelDataManager.h>

void BarrierItem::Init() {
	attackTimer_.Start();

	color_ = 0xFF0000FF;
	eulerRot_.y = 90._deg;

	model_ = ModelDataManager::GetInstance()->LoadObj("Barrier");
}

void BarrierItem::Update([[maybe_unused]] const float deltaTime) {
	attackTimer_.Update(deltaTime);
}

void BarrierItem::Draw() {
	static auto *const blockManager = BlockManager::GetInstance();

	eulerRot_.z = std::atan2(direction_.y, direction_.x);
	transformMat_ = Matrix4x4::MakeRotateMatrix(eulerRot_, Matrix4x4::RotateType::kXYZ);

	*reinterpret_cast<Vector3 *>(transformMat_.m[3]) += *reinterpret_cast<const Vector3 *>(parent_->GetTransMat().m[3]);

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transformMat_, .color_ = color_ });
}

void BarrierItem::Attack(const Vector2 direction) {

	attackTimer_.Start();
	direction_ = direction;

}
