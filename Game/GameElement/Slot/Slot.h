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

enum FaceType
{
	kSad, // 青
	kOko, // 赤
	kNormal, // 緑
	kGekiOko, // 今は亡き新たな顔
	kBad, // はずれ。プレイヤーにとってチャンス
};

class Slot {
public:

	Slot();

	void Initialize();
	void ClearInit();

	void PostEffectWright(Camera *camera);

	void Update(Camera *camera);

	void Draw(Camera *camera);

public:

	// ルーレットを回し始める関数。赤の顔から
	void StartRotation();

	// ルーレットを止め始める関数
	void StopRotation();

	// 回っている顔のレベルを下げる
	void DownLevel();

	// 顔のタイプ
	const FaceType GetFaceType() const { return static_cast<FaceType>(faceType_); }

	// これは分けたけどまだ書いてない
	void StartGekiOkoRotation();

	auto &GetTransform() { return plane_->transform_; }

private:

	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void Rotation();

	void Rotation2();

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

	float timeCount_;

	std::array<int, 3> faceTypes_;
};
