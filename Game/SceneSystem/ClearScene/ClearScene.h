#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"

class ClearScene : public IScene
{
public:
	ClearScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	std::unique_ptr<Sprite> clearSprite_;
};
