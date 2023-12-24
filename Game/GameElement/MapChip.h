#pragma once
#include <list>
#include "../Collision/Collision.h"
#include "BlockManager.h"
#include "../Engine/Utils/SoUtils/IO/CSV.h"
#include "../Engine/Utils/SoUtils/Containers/Array2D.h"
#include <Model.h>

class MapChip
{
public:
	MapChip() = default;
	~MapChip() = default;

	void Init();

	void Load(const SoLib::IO::CSV &csv);

	void Update(const float deltaTime);

	void Draw();

private:

	const float kChipScale_ = 1.f;
	const float kBoxScale = 0.5f;

	BlockManager *pBlockManager_;
	uint32_t model_;

	SoLib::Containers::Array2D<uint32_t> mapData_;

	std::list<AABB> boxList_;

};