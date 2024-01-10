#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Model.h"
#include "Sprite.h"
#include <vector>
#include "Skydome/Skydome.h"
#include "Stage/Stage.h"
#include "Screen/Screen.h"

class TitleScene : public IScene
{
public:
	TitleScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	std::vector<uint32_t> soundHandles_;
	std::vector<uint32_t> voiceHandles_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Screen> screen_;


	std::unique_ptr<Sprite> title_;
	std::unique_ptr<Sprite> space_;
};

