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
#include "Punch.h"
#include "Needle.h"

class Stage {
public:
	Stage() {}
	~Stage() = default;

	void Init();

	void LoadCSV(const SoLib::IO::CSV &csv);

	void SetWeapon();

	void Update(const float deltaTime);

	void Draw();

	MapChip *GetMapChip() const { return mapChip_.get(); }

	auto *const GetBallList() { return &ballList_; }
	auto *const GetSpawnerList() { return &spawnerList_; }

	auto *const GetPinList() { return &pinList_; }
	auto *const GetHoleList() { return &holeList_; }

	auto *const GetSwordList() { return &swordList_; }
	auto *const GetPunchList() { return &punchList_; }

	auto *const GetNeedleList() { return &needleList; }

	void LoadValue(const char *const groupName);
	void SaveValue(const char *const groupName) const;

	void ImGuiWidget();

	float GetHoleChathTime() const { return vHoleChathTime_.GetItem(); }

	const auto &GetSwordEase() const { return vSwordEasing_.GetItem(); }

	float GetSwingTime() const { return vSwingTime_.GetItem(); }

	float GetSwordDamage() const { return vSwordDamage_.GetItem(); }

	const auto &GetPunchEase() const { return vPunchEasing_.GetItem(); }

	float GetNeedleTime() const { return vNeedleTime_.GetItem(); }

	float GetNeedleHight() const { return vNeedleHight_; }

	float GetNeedleScale() const { return vNeedleScale_; }

	float stageGround_ = 0.f;

private:

	uint32_t stageModel_;

	Matrix4x4 transMat_;

	std::list<std::unique_ptr<Needle>> needleList;

	std::list<std::unique_ptr<Punch>> punchList_;

	std::list<std::unique_ptr<Sword>> swordList_;

	std::list<std::unique_ptr<BoundPin>> pinList_;

	std::vector<std::unique_ptr<Hole>> holeList_;

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

	VariantItem<SoLib::EaseFunc> vSwordEasing_ = { "SwordEasing", { SoLib::easeInOutQuad } };

	VariantItem<SoLib::EaseFunc> vPunchEasing_ = { "PunchEasing", { SoLib::easeInOutQuad } };

	VariantItem<SoLib::SecondF> vNeedleTime_{ "NeedleTime", 1.f };

	VariantItem<float> vNeedleHight_{ "NeedleHight", 1.f };
	VariantItem<float> vNeedleScale_{ "NeedleScale", 3.f };

	const std::string groupName_{ "Stage" };

};