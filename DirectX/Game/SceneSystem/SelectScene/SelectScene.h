#pragma once

#include "SceneSystem/IScene/IScene.h"

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

};

