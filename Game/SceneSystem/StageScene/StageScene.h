#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"
#include "Model.h"
#include "Particle.h"
#include "BlockManager.h"
#include <array>

#include "../Engine/Utils/SoUtils/IO/File.h"
#include "../Engine/Utils/SoUtils/IO/CSV.h"
#include <GameElement/MapChip.h>
#include <GameElement/MovingBall.h>
#include <GameElement/Player.h>

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

	std::array<IBlock *, 4> blocks_;

	std::unique_ptr<MovingBall> ball_;
	std::unique_ptr<Player> player_;

	BlockManager *pBlockManager_;
	std::unique_ptr<MapChip> mapChip_;

	SoLib::IO::File file_;
	SoLib::IO::CSV csv_;
};
