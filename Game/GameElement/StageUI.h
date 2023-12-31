#pragma once
#pragma once
#include <memory>
#include "Sprite.h"
#include <array>
#include <string>

class Camera;

class StageUI {
public:

	StageUI();

	void Init();

	void Update();

	void Draw(const Camera& camera);

private:

	void SetGlobalVariable();

	void ApplyGlobalVariable();

	void SetParamaters();

private:

	enum SpriteNames {
		MOVE,
		ATTACK,
		JUMP,
		SPRITE_COUNT,
	};


	std::array<std::unique_ptr<Sprite>, SpriteNames::SPRITE_COUNT> sprites_;

	std::string spriteNames_[SpriteNames::SPRITE_COUNT] = {
		"ムーブの",
		"攻撃の",
		"ジャンプの",
	};

	const std::string groupName_ = "StageUI";

	enum SpriteV2Paramater {
		POSITION,
		SCALE,
		SPRITE_V2_PARAMATER_COUNT,
	};

	std::array<std::array<Vector2, SpriteV2Paramater::SPRITE_V2_PARAMATER_COUNT>, SpriteNames::SPRITE_COUNT> spriteV2Paramaters_;

	std::string spriteV2ParamaterNames_[SpriteV2Paramater::SPRITE_V2_PARAMATER_COUNT] = {
		"ポジション",
		"スケール",
	};
};