#include "Stage.h"

void Stage::Init() {

	mapChip_ = std::make_unique<MapChip>();
	mapChip_->Init();

	ballList_.clear();

}

void Stage::SetCSV(const SoLib::IO::CSV &csv) {
	mapChip_->Load(csv);
}

void Stage::Update(const float deltaTime) {

	mapChip_->Update(deltaTime);

	std::erase_if(ballList_, [](const std::unique_ptr<MovingBall> &ball) ->bool
		{
			return not ball->GetIsAlive();
		});


	for (auto &ball : ballList_) {
		ball->Update(deltaTime);
	}

}

void Stage::Draw() {

	mapChip_->Draw();

	for (auto &ball : ballList_) {
		ball->Draw();
	}
}
