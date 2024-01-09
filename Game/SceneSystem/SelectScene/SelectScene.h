#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Skydome/Skydome.h"
#include "Stage/Stage.h"
#include "Screen/Screen.h"
#include <array>
#include "Sprite.h"

class SelectScene : public IScene
{
public:
	SelectScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	std::vector<uint32_t> soundHandles_;
	std::vector<uint32_t> voiceHandles_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Screen> screen_;

	std::array<std::unique_ptr<Sprite>, COUNT_STAGE> musicSprites_;
};

