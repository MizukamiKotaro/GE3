#include "Stage.h"

void Stage::Init() {

	mapChip_ = std::make_unique<MapChip>();
	mapChip_->Init();

}

void Stage::SetCSV(const SoLib::IO::CSV &csv) {
	mapChip_->Load(csv);
}

void Stage::Update(const float deltaTime) {

	mapChip_->Update(deltaTime);
}

void Stage::Draw() {

	mapChip_->Draw();
}
