#include "StageUI.h"
#include "GlobalVariables/GlobalVariables.h"
#include "Camera.h"

StageUI::StageUI()
{
	sprites_[SpriteNames::MOVE] = std::make_unique<Sprite>("Resources/move.png");
	sprites_[SpriteNames::JUMP] = std::make_unique<Sprite>("Resources/jump.png");
	sprites_[SpriteNames::ATTACK] = std::make_unique<Sprite>("Resources/attack.png");

	SetGlobalVariable();

	SetParamaters();
}

void StageUI::Init()
{

}

void StageUI::Update()
{
#ifdef _DEBUG
	ApplyGlobalVariable();
	SetParamaters();
#endif // _DEBUG
}

void StageUI::Draw(const Camera& camera)
{
	for (const std::unique_ptr<Sprite>& sprite : sprites_) {
		sprite->Draw(camera);
	}
}

void StageUI::SetGlobalVariable()
{
#ifdef _DEBUG

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	globalVariables->CreateGroup(groupName_);

	for (uint32_t i = 0; i < SpriteNames::SPRITE_COUNT; i++) {
		for (uint32_t j = 0; j < SpriteV2Paramater::SPRITE_V2_PARAMATER_COUNT; j++) {
			globalVariables->AddItem(groupName_, spriteNames_[i] + spriteV2ParamaterNames_[j], spriteV2Paramaters_[i][j]);
		}
	}

#endif // _DEBUG

	ApplyGlobalVariable();
}

void StageUI::ApplyGlobalVariable()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	for (uint32_t i = 0; i < SpriteNames::SPRITE_COUNT; i++) {
		for (uint32_t j = 0; j < SpriteV2Paramater::SPRITE_V2_PARAMATER_COUNT; j++) {
			spriteV2Paramaters_[i][j] = globalVariables->GetVector2Value(groupName_, spriteNames_[i] + spriteV2ParamaterNames_[j]);
		}
	}
}

void StageUI::SetParamaters()
{
	for (uint32_t i = 0; i < SpriteNames::SPRITE_COUNT; i++) {
		sprites_[i]->pos_ = spriteV2Paramaters_[i][SpriteV2Paramater::POSITION];
		//sprites_[i]->size_ = spriteV2Paramaters_[i][SpriteV2Paramater::SCALE];
		sprites_[i]->Update();
	}
}
