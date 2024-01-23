#pragma once
#include <memory>
#include "BlockManager.h"
#include "SoUtils/Containers/Singleton.h"

#include "Shape/Sphere.h"
#include "Shape/Capsule.h"

class CollisionRenderer : public SoLib::Singleton<CollisionRenderer> {
	friend SoLib::Singleton<CollisionRenderer>;
	CollisionRenderer() = default;
	~CollisionRenderer() = default;
public:

	void Init();

	void LoadModel();

	void clear();

	/// @brief 描画するかどうか(書き込み前に設定)
	/// @param isDraw 描画するかの真偽値
	inline void SetIsDraw(const bool isDraw) { isDraw_ = isDraw; }

	void AddCollision(const Sphere &sphere);
	void AddCollision(const Capsule &capsule);

	bool ImGuiWidget(const char *const label);


private:

	uint32_t sphere_;
	uint32_t cylinder_;


	BlockManager *pBlockManager_ = nullptr;

	bool isDraw_ = false;

};