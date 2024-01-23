#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"

class TitleScene : public IScene
{
public:
	TitleScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	std::unique_ptr<Sprite> titleSprite_;
};

