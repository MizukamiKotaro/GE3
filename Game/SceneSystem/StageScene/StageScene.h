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

#include "PostEffect/Blur/Blur.h"
#include <SoUtils/Graphics/CollisionRenderer.h>
#include <string>
#include <GameElement/Boss.h>
#include "GameElement/HPBar/HPBar.h"

class StageScene : public IScene {
public:

	StageScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	void CreatePostEffects();

private:

	std::unique_ptr<Player> player_;

	CollisionRenderer *collisionRenderer_;

	BlockManager *pBlockManager_;
	std::unique_ptr<Stage> stage_;

	SoLib::IO::CSV csv_;

	std::unique_ptr<StageUI> stageUI_;

	std::unique_ptr<Boss> boss_;

	//std::unique_ptr<Slot> slot_;

	std::unique_ptr<Blur> swordBlur_;
	bool isDrawSwordBlur_;

	std::unique_ptr<Blur> needleBlur_;
	bool isDrawNeedleBlur_;

	std::unique_ptr<Blur> punchBlur_;
	bool isDrawPunchBlur_;

	std::unique_ptr<HPBar> playerHPBar_;
	std::unique_ptr<HPBar> bossHPBar_;
};
