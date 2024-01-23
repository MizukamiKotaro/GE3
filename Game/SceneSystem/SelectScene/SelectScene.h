#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"

class SelectScene : public IScene
{
public:
	SelectScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	std::unique_ptr<Sprite> selectSprite_;

};

