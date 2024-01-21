#pragma once
#include <memory>
#include "BlockManager.h"

#include "Shape/Sphere.h"
#include "Shape/Capsule.h"

class CollisionRenderer {
	CollisionRenderer() = default;
	CollisionRenderer(const CollisionRenderer &) = delete;
	CollisionRenderer &operator=(const CollisionRenderer &) = delete;

	~CollisionRenderer() = default;
public:

	static CollisionRenderer *const GetInstance();

	void Init();

	void LoadModel();

	void clear();

	/// @brief 描画するかどうか(書き込み前に設定)
	/// @param isDraw 描画するかの真偽値
	inline void SetIsDraw(const bool isDraw) { isDraw_ = isDraw; }

	void AddCollision(const Sphere &sphere);
	void AddCollision(const Capsule &capsule);


private:
	struct SingletonDeleter {
		void operator()(CollisionRenderer *ptr) const {
			delete ptr;
		}
	};

	static std::unique_ptr<CollisionRenderer, SingletonDeleter> instance_;

	uint32_t sphere_;
	uint32_t cylinder_;


	BlockManager *pBlockManager_ = nullptr;

	bool isDraw_ = true;

};