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

class Camera;

class Decoration {
public:

	Decoration();

	void Initialize();

	void Update(Camera* camera);

	void Draw(Camera* camera);

public:

	void WrightPostEffect(Camera* camera);

	void SetIsSword(const bool* isSword) { isSword_ = isSword; }

	void SetIsNeedle(const bool* isNeedle) { isNeedle_ = isNeedle; }

	void SetIsPunch(const bool* isPunch) { isPunch_ = isPunch; }

	void SetIsRight(const bool* isRight) { isRight_ = isRight; }

private:

private:

	enum ModelType {
		Sword,
		SwordB,
		Punch,
		PunchB,
		Needle,
		EndModelType,
	};

	const bool* isSword_ = nullptr;
	const bool* isNeedle_ = nullptr;
	const bool* isPunch_ = nullptr;

	const bool* isRight_ = nullptr;

	/*std::unique_ptr<Model> swordDecration_;
	std::unique_ptr<Model> swordDecrationB_;

	std::unique_ptr<Model> punchDecration_;
	std::unique_ptr<Model> punchDecrationB_;

	std::unique_ptr<Model> needleDecration_;*/

	std::array<std::unique_ptr<Model>, EndModelType> decrations_;

	std::array<std::unique_ptr<Model>, EndModelType> numbers_;
	std::array<std::unique_ptr<Model>, EndModelType> denominators_;
	std::array<std::unique_ptr<Model>, EndModelType> slashes_;
	std::array<uint32_t, 6> numberModelDatas_;

	std::unique_ptr<HighLumi> highLumi_;

	//std::unique_ptr<GaussianBlur> gaussian_;

	std::unique_ptr<PostEffect> post_;

	std::unique_ptr<Sprite> rainbow_;
	float texcoodY_;

	bool isFirst_;
};