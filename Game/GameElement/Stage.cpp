#include "Stage.h"

void Stage::Init() {

	mapChip_ = std::make_unique<MapChip>();
	mapChip_->Init();

	ballList_.clear();

}

void Stage::LoadCSV(const SoLib::IO::CSV &csv) {
	mapChip_->Load(csv);
	const auto posList = mapChip_->GetChipPos(MapChip::ChipType::kPin);

	for (const auto &pos : posList) {

		auto pinItem = pinList_.emplace_back(std::make_unique<BoundPin>()).get();

		pinItem->Init();
		pinItem->SetPos(pos);

	}

}

void Stage::Update(const float deltaTime) {

	mapChip_->Update(deltaTime);

	std::erase_if(spawnerList_, [](const std::unique_ptr<BallSpawner> &spawner) ->bool
		{
			return not spawner->GetIsAlive();
		}
	);

	std::erase_if(ballList_, [](const std::unique_ptr<MovingBall> &ball) ->bool
		{
			return not ball->GetIsAlive();
		}
	);

	for (auto &pin : pinList_) {
		pin->Update(deltaTime);
	}

	for (auto &spawner : spawnerList_) {
		spawner->Update(deltaTime);
	}

	for (auto &ball : ballList_) {
		ball->Update(deltaTime);
	}

}

void Stage::Draw() {

	mapChip_->Draw();

	for (auto &pin : pinList_) {
		pin->Draw();
	}

	for (auto &spawner : spawnerList_) {
		spawner->Draw();
	}

	for (auto &ball : ballList_) {
		ball->Draw();
	}
}
