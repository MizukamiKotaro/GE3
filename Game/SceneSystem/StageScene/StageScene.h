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

#include "GameElement/StageUI.h"
#include <GameElement/Stage.h>

#include "GameElement/Slot/Slot.h"

class StageScene : public IScene {
public:

	StageScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	//std::list<std::unique_ptr<MovingBall>> ballList_;
	std::unique_ptr<Player> player_;

	BlockManager *pBlockManager_;
	std::unique_ptr<Stage> stage_;

	SoLib::IO::File file_;
	SoLib::IO::CSV csv_;

	std::unique_ptr<StageUI> stageUI_;

	std::unique_ptr<Slot> slot_;
};
