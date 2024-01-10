#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"
#include "Model.h"
#include "Particle.h"
#include "Light/Light.h"
#include <array>
#include "PostEffect/PostEffect.h"
#include "Skydome/Skydome.h"
#include "Stage/Stage.h"
#include "Screen/Screen.h"
#include "StageLights/StageLights.h"
#include "Score/Score.h"

class StageScene : public IScene
{
public:

	StageScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	void Play();

	void HitTest();

	void UIDraw();

private:

	float time_;

	uint32_t musicHundle_;
	uint32_t musicVoice_;

	bool isStartMusic_;

	float finishCount_;

	const int kNumSize = 32;
	int comCount_ = 0;
	std::array<std::unique_ptr<Sprite>, 2> comboNum_;

	std::unique_ptr<Sprite> hitSp_;
	const int kHitSizeX_ = 220;
	const int kHitSizeY_ = 64;
	int hitCount_ = 0;
	int hitNum_ = 0;

	enum HitErrorFrame {
		kPerfect,
		kGreat,
		kGood,
		kMiss,
	};
	const float kHitErrorFrame_[3] = { 5.0f / 60, 0.6f, 14.0f / 60 };

	bool isMusicFinish_;

	std::array<std::unique_ptr<Sprite>,3> sp_;

	std::array<std::unique_ptr<Sprite>, 4> scoreNum_;

	std::unique_ptr<Score> score_;

	std::unique_ptr<Sprite> screenSprite_;

	std::unique_ptr<PostEffect> postEffect_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Screen> screen_;

	std::unique_ptr<Screen> postScreen_;

	std::unique_ptr<StageLights> stageLights_;

	uint32_t se[3];

	std::unique_ptr<DirectionalLight> directionalLight_;

	std::unique_ptr<PointLight> pointLight_;

	std::unique_ptr<SpotLight> spotLight_;

	std::unique_ptr<Model> model;

	std::unique_ptr<Particle> particle;
};
