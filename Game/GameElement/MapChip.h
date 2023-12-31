#pragma once
#include <list>
#include "../Collision/Collision.h"
#include "BlockManager.h"
#include "../Engine/Utils/SoUtils/IO/CSV.h"
#include "../Engine/Utils/SoUtils/Containers/Array2D.h"
#include <Model.h>

class MapChip {
public:

	enum class ChipType : uint32_t {
		kAir = 0b00,
		kBlock = 0b01,
		kPin = 0b10,
	};

public:
	MapChip() = default;
	~MapChip() = default;

	void Init();

	void Load(const SoLib::IO::CSV &csv);

	void Transfer();

	void Update(const float deltaTime);

	void Draw();

	const std::list<AABB> &GetBoxList() const { return boxList_; }

	Vector3 HitMap(const Vector3 &beforePos, const Vector3 &afterPos, float) const;

private:

	void TransferBox();

	const float kChipScale_ = 1.f;
	const float kBoxScale = 0.5f;

	BlockManager *pBlockManager_;
	uint32_t model_;

	Vector3 offset_{};

	SoLib::Containers::Array2D<uint32_t> mapData_;

	std::list<AABB> boxList_;

};