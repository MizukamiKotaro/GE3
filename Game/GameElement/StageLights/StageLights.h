#pragma once
#include "Light/Light.h"
#include <memory>
#include <array>
#include "PostEffect/PostEffect.h"
#include "PostEffect/Blur/Blur.h"
#include "GlobalVariables/GlobalVariables.h"
#include <string>

class Camera;

class StageLights
{
public:
	StageLights();

	void Initialize();

	void Update();

	void Draw(const Camera* camera);

private:

	void SetGlobalVariable();

	void ApplyGlobalVariable();

private:

	GlobalVariables* globalVariable_;

	const std::string groupName_ = "Lights";

	enum EnumFInfo {
		Radius,
		PosZ,
		FEndCount,
	};

	float fInfo_[FEndCount];

	std::string fItemNames[FEndCount] = {
		"半径",
		"z座標"
	};

	enum EnumV2Info {
		Space,
		V2EndCount,
	};

	Vector2 v2Info_[V2EndCount];

	std::string v2ItemNames[V2EndCount] = {
		"間隔"
	};

	/*enum EnumV3Info {
		Pos,
		V3EndCount,
	};

	Vector3 v3Info_[V3EndCount];

	std::string v3ItemNames[V3EndCount] = {
		"ポジション"
	};*/

private:

	static const int heightNum_ = 5;
	static const int sideNum_ = 3;

private:

	std::array<std::array<std::unique_ptr<PointLight>, heightNum_>, 2> heightLights_;
	std::array<std::array<std::unique_ptr<PointLight>, 2>, sideNum_ - 2> topDownLights_;



};
