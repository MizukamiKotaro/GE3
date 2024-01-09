#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"
#include <memory>
#include "Skydome/Skydome.h"
#include "Stage/Stage.h"
#include "Screen/Screen.h"

class ClearScene : public IScene
{
public:
	ClearScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Screen> screen_;

	std::unique_ptr<Sprite> space_;

	std::unique_ptr<Sprite> sp_[3];

	std::unique_ptr<Sprite> scoreNum_[4];
	const int kNumSize = 32;

	std::unique_ptr<Sprite> comboNum_[2];

	std::unique_ptr<Sprite> perNum_[2];

	std::unique_ptr<Sprite> greNum_[2];

	std::unique_ptr<Sprite> goodNum_[2];

	std::unique_ptr<Sprite> missNum_[2];

	std::unique_ptr<Sprite> hitSp_[4];

	std::unique_ptr<Sprite> s_;

	int count_ = 0;
};
