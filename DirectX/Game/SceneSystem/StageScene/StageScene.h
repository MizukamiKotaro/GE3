#pragma once

#include "SceneSystem/IScene/IScene.h"

class StageScene : public IScene
{
public:

	StageScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:


};
