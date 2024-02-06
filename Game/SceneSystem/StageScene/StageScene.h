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

#include "GameElement/TitleObject/TitleObject.h"
#include "PostEffect/PostEffect.h"
#include "GameElement/Decoration/Decoration.h"

class StageScene : public IScene {
public:

	StageScene();

	void Init() override;
	void Update() override;
	void Draw() override;

private:

	void TitleUpdate(float deltaTime);

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
	bool isRight_;

	std::unique_ptr<Blur> swordBlur_;
	bool isDrawSwordBlur_;
	bool isSwordAttack_;

	std::unique_ptr<Blur> needleBlur_;
	bool isDrawNeedleBlur_;
	bool isNeedleAttack_;

	std::unique_ptr<Blur> punchBlur_;
	bool isDrawPunchBlur_;
	bool isPunchAttack_;

	std::unique_ptr<Blur> playerBlur_;
	bool isPlayerAttack_;

	std::unique_ptr<HPBar> playerHPBar_;
	std::unique_ptr<HPBar> bossHPBar_;

	std::unique_ptr<TitleObject> titleObj_;

	Vector3 titleCameraPos_;
	bool isTitle_ = true;
	bool isStart_ = false;
	float countEaseTime_;

	std::unique_ptr<PostEffect> postEffect_;
	std::array<std::unique_ptr<Sprite>, 2> slotMirrors_;
	float slotMirrorsPosX_;

	std::unique_ptr<Decoration> decoration_;
	std::unique_ptr<PostEffect> farPostEffect_;

	std::unique_ptr<Sprite> space_;

	AudioItem se_;
	AudioItem BGM;
};
