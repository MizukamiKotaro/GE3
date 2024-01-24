#include "HPBar.h"
#include "Camera.h"
#include "TextureManager.h"
#include <algorithm>
#include "FrameInfo/FrameInfo.h"
#include "Input.h"

HPBar::HPBar(const std::string& groupName)
{
	groupName_ = groupName;

	v2Info_[EnumV2Info::Size] = { 400.0f,50.0f };
	v2Info_[EnumV2Info::Position] = { 640.0f, 60.0f };
	v2Info_[EnumV2Info::Interval] = { 5.0f,5.0f };
	v2Info_[EnumV2Info::Offset] = { 0.0f,-30.0f };

	globalVariable_ = GlobalVariables::GetInstance();


	for (int i = 0; i < SpriteNames::kSpriteCount; i++) {
		sprites_[i] = std::make_unique<Sprite>("Resources/white.png");
		sprites_[i]->size_ = v2Info_[EnumV2Info::Size];
		if (i == SpriteNames::kFrame) {
			sprites_[i]->size_ += v2Info_[EnumV2Info::Interval];
		}
		else if (i == SpriteNames::kDamage) {
			sprites_[i]->size_.x = 0.0f;
		}
		sprites_[i]->pos_ = v2Info_[EnumV2Info::Position] - sprites_[i]->size_ / 2.0f;
		sprites_[i]->SetAnchorPoint({ 0.0f,0.0f });
	}

	sprites_[SpriteNames::kFrame]->SetColor({ 0.1f,0.1f,0.1f,1.0f });
	sprites_[SpriteNames::kHP]->SetColor({ 0.2f,1.0f,0.4f,1.0f });
	sprites_[SpriteNames::kBack]->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	sprites_[SpriteNames::kDamage]->SetColor({ 0.8f,0.4f,0.2f,1.0f });

	SetGlobalVariable();

	for (int i = 0; i < SpriteNames::kSpriteCount; i++) {
		sprites_[i]->Update();
	}
	isDamage_ = false;
}

void HPBar::Initialize()
{
	for (int i = 0; i < SpriteNames::kSpriteCount; i++) {
		sprites_[i]->size_ = v2Info_[EnumV2Info::Size];
		if (i == SpriteNames::kFrame) {
			sprites_[i]->size_ += v2Info_[EnumV2Info::Interval];
		}
		else if (i == SpriteNames::kDamage) {
			sprites_[i]->size_.x = 0.0f;
		}
		sprites_[i]->pos_ = v2Info_[EnumV2Info::Position] - sprites_[i]->size_ / 2.0f;
	}
	HPSize_ = sprites_[SpriteNames::kHP]->size_.x;
	isDamage_ = false;

	time_ = 0.0f;
}

void HPBar::Update(Camera* camera)
{
#ifdef _DEBUG
	ApplyGlobalVariable();

	if (Input::GetInstance()->PressedKey(DIK_LSHIFT)) {
		SetDamage(2.0f);
	}
	if (Input::GetInstance()->PressedKey(DIK_Q)) {
		SetMaxHP(10.0f);
	}
	sprites_[SpriteNames::kHP]->size_.x = HPSize_;
	sprites_[SpriteNames::kDamage]->pos_.x = sprites_[SpriteNames::kHP]->pos_.x + sprites_[SpriteNames::kHP]->size_.x;
#endif // _DEBUG



	if (parent_) {
		Vector3 screenPos = (*parent_) * camera->GetViewProjection();
		Vector2 pos = { screenPos.x,screenPos.y };

		for (int i = 0; i < SpriteNames::kSpriteCount; i++) {
			sprites_[i]->pos_ = pos + v2Info_[EnumV2Info::Interval] - sprites_[i]->size_ / 2.0f;
		}
		sprites_[SpriteNames::kDamage]->pos_.x = sprites_[SpriteNames::kHP]->pos_.x + sprites_[SpriteNames::kHP]->size_.x;
	}


	if (isDamage_) {
		time_ += FrameInfo::GetInstance()->GetDeltaTime();

		float t = std::clamp(time_, 0.0f, 0.5f) / 0.5f;
		sprites_[SpriteNames::kDamage]->size_.x = damageSize_ * (1.0f - t);

		if (time_ >= 0.5f) {
			time_ = 0.0f;
			isDamage_ = false;
		}
	}

	for (int i = 0; i < SpriteNames::kSpriteCount; i++) {
		sprites_[i]->Update();
	}
}

void HPBar::Draw()
{
	for (int i = 0; i < SpriteNames::kSpriteCount; i++) {
		sprites_[i]->Draw();
	}
}

void HPBar::SetMaxHP(float maxHP)
{ 
	maxHP_ = maxHP;
	HP_ = maxHP_;
}

void HPBar::SetDamage(float damage)
{
	if (HP_ > 0.0f) {
		sprites_[SpriteNames::kDamage]->size_.x += v2Info_[EnumV2Info::Size].x * std::clamp(damage, 0.0f, maxHP_) / maxHP_;
		damageSize_ = sprites_[SpriteNames::kDamage]->size_.x;

		HP_ -= damage;
		if (HP_ < 0.0f) {
			HP_ = 0.0f;
		}
		time_ = 0.0f;

		sprites_[SpriteNames::kHP]->size_.x = v2Info_[EnumV2Info::Size].x * HP_ / maxHP_;
		HPSize_ = sprites_[SpriteNames::kHP]->size_.x;

		sprites_[SpriteNames::kDamage]->pos_.x = sprites_[SpriteNames::kHP]->pos_.x + sprites_[SpriteNames::kHP]->size_.x;

		sprites_[SpriteNames::kHP]->Update();
		sprites_[SpriteNames::kDamage]->Update();

		isDamage_ = true;
	}
}

void HPBar::SetGlobalVariable()
{
	globalVariable_->CreateGroup(groupName_);

	for (int i = 0; i < EnumV2Info::V2EndCount; i++) {
		globalVariable_->AddItem(groupName_, v2ItemNames[i], v2Info_[i]);
	}

	ApplyGlobalVariable();
}

void HPBar::ApplyGlobalVariable()
{
	for (int i = 0; i < EnumV2Info::V2EndCount; i++) {
		v2Info_[i] = globalVariable_->GetVector2Value(groupName_, v2ItemNames[i]);
	}

	for (int i = 0; i < SpriteNames::kSpriteCount; i++) {
		sprites_[i]->size_ = v2Info_[EnumV2Info::Size];
		if (i == SpriteNames::kFrame) {
			sprites_[i]->size_ += v2Info_[EnumV2Info::Interval];
		}
		else if (i == SpriteNames::kDamage) {
			sprites_[i]->size_.x = 0.0f;
		}
		sprites_[i]->pos_ = v2Info_[EnumV2Info::Position] - sprites_[i]->size_ / 2.0f;
	}

	sprites_[SpriteNames::kDamage]->pos_.x = sprites_[SpriteNames::kHP]->pos_.x + sprites_[SpriteNames::kHP]->size_.x;
}
