#pragma once
#include <memory>
#include <array>
#include <string>
#include "GlobalVariables/GlobalVariables.h"
#include "Model.h"
#include "Sprite.h"
#include "PostEffect/HighLumi/HighLumi.h"
#include "PostEffect/GaussianBlur/GaussianBlur.h"
#include "PostEffect/PostEffect.h"
#include "Audio.h"

class Camera;

class Decoration {
public:

	Decoration();

	void Initialize();

	void Update(Camera* camera);

	void tcUpdate(Camera* camera);

	void Draw(Camera* camera);

	void tcDraw(Camera* camera);

public:

	void WrightPostEffect(Camera* camera);

	void SetIsSword(const bool* isSword) { isSword_ = isSword; }

	void SetIsNeedle(const bool* isNeedle) { isNeedle_ = isNeedle; }

	void SetIsPunch(const bool* isPunch) { isPunch_ = isPunch; }

	void SetIsRight(const bool* isRight) { isRight_ = isRight; }

private:
	void Wright(Camera* camera);

	enum ModelType {
		Sword,
		SwordB,
		Punch,
		PunchB,
		Needle,
		EndModelType,
	};

	void ChangeGaugeColor();

private:


	static const int kMaxGauge_ = 5;

	const bool* isSword_ = nullptr;
	const bool* isNeedle_ = nullptr;
	const bool* isPunch_ = nullptr;

	const bool* isRight_ = nullptr;

	std::array<std::unique_ptr<Model>, EndModelType> decrations_;

	std::array<std::unique_ptr<Model>, EndModelType> numbers_;
	std::array<std::array<std::unique_ptr<Model>, kMaxGauge_>, EndModelType> gauges_;
	std::array<uint32_t, 6> numberModelDatas_;
	std::array<int, EndModelType> nums_;

	std::array<bool, EndModelType> isGimmick_;
	AudioItem se_;
	float volume_;

	std::unique_ptr<HighLumi> highLumi_;

	//std::unique_ptr<GaussianBlur> gaussian_;

	std::unique_ptr<PostEffect> post_;

	std::unique_ptr<Sprite> rainbow_;
	float texcoodY_;

	bool isFirst_;
};