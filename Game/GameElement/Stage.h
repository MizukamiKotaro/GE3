#pragma once
#include "MapChip.h"
#include "MovingBall.h"
#include <memory>
#include "BallSpawner.h"

class Stage {
public:
	Stage() = default;
	~Stage() = default;

	void Init();

	void SetCSV(const SoLib::IO::CSV &csv);

	void Update(const float deltaTime);

	void Draw();

	MapChip *GetMapChip() const { return mapChip_.get(); }

	auto *const GetBallList() { return &ballList_; }
	auto *const GetSpawnerList() { return &spawnerList_; }

private:

	std::list<std::unique_ptr<BallSpawner>> spawnerList_;

	std::list<std::unique_ptr<MovingBall>> ballList_;

	std::unique_ptr<MapChip> mapChip_ = nullptr;

};