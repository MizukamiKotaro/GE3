#pragma once
#include "MapChip.h"
#include "MovingBall.h"
#include <memory>
#include "BallSpawner.h"
#include "BoundPin.h"
#include "SoUtils/IO/VariantItem.h"
#include <SoUtils/Math/Euler.h>
#include "Hole.h"
#include "Sword.h"
#include "SoUtils/Math/Angle.h"

class Stage {
public:
	Stage() {}
	~Stage() = default;

	void Init();

	void LoadCSV(const SoLib::IO::CSV &csv);

	void Update(const float deltaTime);

	void Draw();

	MapChip *GetMapChip() const { return mapChip_.get(); }

	auto *const GetBallList() { return &ballList_; }
	auto *const GetSpawnerList() { return &spawnerList_; }

	auto *const GetPinList() { return &pinList_; }
	auto *const GetHoleList() { return &holeList_; }

	auto *const GetSwordList() { return &swordList_; }

	void LoadValue(const char *const groupName);
	void SaveValue(const char *const groupName) const;

	void ImGuiWidget();

	float GetHoleChathTime() const { return vHoleChathTime_.GetItem(); }

	const auto &GetSwordEase() const { return kSwordEasing_.GetItem(); }

	float GetSwingTime() const { return vSwingTime_.GetItem(); }

	float GetSwordDamage() const { return vSwordDamage_.GetItem(); }

private:

	uint32_t stageModel_;

	Matrix4x4 transMat_;

	std::list<std::unique_ptr<Sword>> swordList_;

	std::list<std::unique_ptr<BoundPin>> pinList_;

	std::list<std::unique_ptr<Hole>> holeList_;

	std::list<std::unique_ptr<BallSpawner>> spawnerList_;

	std::list<std::unique_ptr<MovingBall>> ballList_;

	std::unique_ptr<MapChip> mapChip_ = nullptr;

	VariantItem<Vector3> scale_{ "Scale",Vector3::one };
	VariantItem<SoLib::Math::Euler> rotate_{ "Rotate" };
	VariantItem<Vector3> position_{ "Position" };

	VariantItem<float> vSwordDamage_{ "SwordDamage", 1.f };
	VariantItem<SoLib::SecondF> vSwingTime_{ "SwingTime", 1.f };

	VariantItem<float> vHoleSize_{ "HoleSize", 0.75f };
	VariantItem<SoLib::SecondF> vHoleChathTime_{ "HoleChathTime", 1.f };

	VariantItem<SoLib::EaseFunc> kSwordEasing_ = { "SwordEasing", { SoLib::easeInOutQuad } };

	std::string groupName_{ "Stage" };

};