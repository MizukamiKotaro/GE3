#include "Stage.h"
#include <ModelDataManager.h>
#include <GlobalVariables/GlobalVariables.h>

void Stage::Init() {

	LoadValue(groupName_.c_str());

	mapChip_ = std::make_unique<MapChip>();
	mapChip_->Init();

	ballList_.clear();

	transMat_ = Matrix4x4::MakeIdentity4x4();

	stageModel_ = ModelDataManager::GetInstance()->LoadObj("stage");

	SaveValue(groupName_.c_str());

}

void Stage::LoadCSV(const SoLib::IO::CSV &csv) {
	mapChip_->Load(csv);
	const auto posList = mapChip_->GetChipPos(MapChip::ChipType::kPin);

	for (const auto &pos : posList) {

		auto pinItem = pinList_.emplace_back(std::make_unique<BoundPin>()).get();

		pinItem->Init();
		pinItem->SetPos(pos);

	}
	const auto holePos = mapChip_->GetChipPos(MapChip::ChipType::kHole);

	for (const auto &pos : holePos) {

		auto holeItem = holeList_.emplace_back(std::make_unique<Hole>()).get();

		holeItem->Init();
		holeItem->SetPos(pos);

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

	for (auto &hole : holeList_) {
		hole->Update(deltaTime);
	}

	transMat_ = Matrix4x4::MakeAffinMatrix(scale_, rotate_.GetItem(), position_);
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

	for (auto &hole : holeList_) {
		hole->Draw();
	}

	static auto *const blockManager = BlockManager::GetInstance();

	blockManager->AddBox(stageModel_, IBlock{ .transformMat_ = transMat_,.color_ = 0xFFFFFFFF });

}

void Stage::LoadValue(const char *const groupName) {
	static GlobalVariables *const gVariables = GlobalVariables::GetInstance();

	scale_ = gVariables->Get<Vector3>(groupName, scale_.GetKey());

	rotate_ = gVariables->Get<Vector3>(groupName, rotate_.GetKey());

	position_ = gVariables->Get<Vector3>(groupName, position_.GetKey());

}

void Stage::SaveValue(const char *const groupName) const {
	static GlobalVariables *const gVariables = GlobalVariables::GetInstance();

	gVariables->SetValue(groupName, scale_.GetKey(), scale_);
	gVariables->SetValue(groupName, rotate_.GetKey(), rotate_.GetItem());
	gVariables->SetValue(groupName, position_.GetKey(), position_);

}

void Stage::ImGuiWidget() {
	static GlobalVariables *const gVariables = GlobalVariables::GetInstance();

	if (ImGui::TreeNode("StageEditor")) {

		SoLib::ImGuiWidget(&scale_);
		SoLib::ImGuiWidget(&rotate_);
		SoLib::ImGuiWidget(&position_);

		if (ImGui::Button("Save")) {

			SaveValue(groupName_.c_str());
			gVariables->SaveFile(groupName_.c_str());
		}

		ImGui::TreePop();
	}
}
