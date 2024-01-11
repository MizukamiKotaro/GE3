#pragma once
#include "MapChip.h"
#include <memory>

class Stage {
public:
	Stage() = default;
	~Stage() = default;

	void Init();

	void SetCSV(const SoLib::IO::CSV &csv);

	void Update(const float deltaTime);

	void Draw();

	MapChip *GetMapChip() const { return mapChip_.get(); }

private:

	std::unique_ptr<MapChip> mapChip_ = nullptr;

};