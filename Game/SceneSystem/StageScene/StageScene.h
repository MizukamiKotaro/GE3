#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"
#include "Model.h"
#include "Particle.h"
#include "BlockManager.h"
#include <array>

class StageScene : public IScene
{
public:

	StageScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Sprite> sprite1;

	uint32_t mesh1;
	uint32_t mesh2;

	bool isMesh1;

	std::unique_ptr<Model> model;

	std::unique_ptr<Particle> particle;

	std::array<IBlock*, 4> blocks_;
};
