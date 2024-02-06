#pragma once

#include "SceneSystem/IScene/IScene.h"
#include "Sprite.h"

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

#include "GameElement/TitleObject/TitleObject.h"
#include "PostEffect/PostEffect.h"
#include "GameElement/Decoration/Decoration.h"
#include "GameElement/ClearPlayer/ClearPlayer.h"

class ClearScene : public IScene
{
public:
	ClearScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	std::unique_ptr<Sprite> clearSprite_;

	BlockManager* pBlockManager_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<ClearPlayer> player_;

	SoLib::IO::CSV csv_;

	std::unique_ptr<Decoration> decoration_;
	bool isSwordAttack_;
	bool isNeedleAttack_;
	bool isPunchAttack_;
	bool isRight_;

	std::unique_ptr<Blur> playerBlur_;
	bool isPlayerAttack_;

	std::unique_ptr<PostEffect> post_;

	std::unique_ptr<Slot> slot_;

	std::unique_ptr<Sprite> space_;

	AudioItem se_;
};
