#pragma once
#include "Light/Light.h"
#include <memory>
#include <array>
#include "Sprite.h"
#include "PostEffect/PostEffect.h"
#include "Model.h"
#include "PostEffect/Blur/Blur.h"
#include "GlobalVariables/GlobalVariables.h"
#include <string>

class Camera;

class Slot {
public:

	Slot();

	void Initialize();
	void PostEffectWright(Camera *camera);

	void Update(Camera *camera);

	void Draw(Camera *camera);

public:

	void StartRotation();

	void StopRotation();

	// 顔のタイプ
	const int GetFaceType() const { return faceType_; }

	auto &GetTransform() { return plane_->transform_; }

private:

	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void Rotation();

	void DownToTop();

private:

	/*GlobalVariables *globalVariable_;

	const std::string groupName_ = "Slot";

	enum EnumV2Info {
		Scale,
		V2EndCount,
	};

	Vector2 v2Info_[V2EndCount];

	std::string v2ItemNames[V2EndCount] = {
		"スケール"
	};

	enum EnumV3Info {
		Pos,
		V3EndCount,
	};

	Vector3 v3Info_[V3EndCount];

	std::string v3ItemNames[V3EndCount] = {
		"ポジション"
	};*/

private:

	std::unique_ptr<PostEffect> postEffect_;

	std::array<std::unique_ptr<Blur>, 3> blurs_;

	std::unique_ptr<Model> plane_;

	std::array<std::array<std::unique_ptr<Sprite>, 3>, 3> slot_;

	std::array<std::unique_ptr<Sprite>, 3> backs_;

	std::unique_ptr<Sprite> back_;

	std::array<float, 3> rotSpeed_;

	std::array<bool, 3> isRotStop_;

	std::array<bool, 3> isAcross_;
	int acrossNum_;

	float limitSpeed_;

	bool isRot_ = false;

	bool isStop_ = true;

	int faceType_;

	float interval_;
};
