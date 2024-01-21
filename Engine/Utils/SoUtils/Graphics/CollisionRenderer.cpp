#include "CollisionRenderer.h"
#include <array>
#include "ModelDataManager.h"

std::unique_ptr<CollisionRenderer, CollisionRenderer::SingletonDeleter> CollisionRenderer::instance_;

CollisionRenderer *const CollisionRenderer::GetInstance() {
	// インスタンスが無ければ生成
	if (not instance_) {
		instance_.reset(new CollisionRenderer);
		instance_->Init();
	}

	return instance_.get();
}

void CollisionRenderer::Init() {

	this->clear();
	LoadModel();

	pBlockManager_ = BlockManager::GetInstance();

}

void CollisionRenderer::LoadModel() {
	auto *const modelManager = ModelDataManager::GetInstance();
	sphere_ = modelManager->LoadObj("Sphere");
	cylinder_ = modelManager->LoadObj("Cylinder");

}

void CollisionRenderer::clear() {

}

void CollisionRenderer::AddCollision(const Sphere &sphere) {

	pBlockManager_->AddBox(sphere_, IBlock{ .transformMat_ = sphere.MakeAffineMatrix(), .color_ = 0xFFFFFF55 });
}

void CollisionRenderer::AddCollision(const Capsule &capsule) {

#pragma region 円錐部

	// 円錐部
	Matrix4x4 cylMat =
		// 回転要素を取得
		Matrix4x4::DirectionToDirection(Vector3::up, capsule.segment_.diff);
	// 平行移動 ( 線分の中心座標を保存 )
	*reinterpret_cast<Vector3 *>(cylMat.m[3]) = capsule.segment_.GetPoint(0.5f);

	// スケール
	// 半径の計算
	*reinterpret_cast<Vector3 *>(cylMat.m[0]) *= capsule.radius_;
	*reinterpret_cast<Vector3 *>(cylMat.m[2]) *= capsule.radius_;
	// 縦の長さの計算
	*reinterpret_cast<Vector3 *>(cylMat.m[1]) *= (capsule.segment_.diff.Length() / 2.f);

	// データの追加
	pBlockManager_->AddBox(cylinder_, IBlock{ .transformMat_ = cylMat, .color_ = 0xFFFFFF55 });

#pragma endregion

#pragma region 球体部

	// 球体のアフィン行列
	std::array<Matrix4x4, 2u> sphereMat{};

	sphereMat[0u] = Sphere::MakeAffineMatrix(capsule.segment_.origin, capsule.radius_);
	sphereMat[1u] = Sphere::MakeAffineMatrix(capsule.segment_.GetEnd(), capsule.radius_);

	for (const auto &sphere : sphereMat) {
		pBlockManager_->AddBox(sphere_, IBlock{ .transformMat_ = sphere, .color_ = 0xFFFFFF55 });
	}


#pragma endregion



}
