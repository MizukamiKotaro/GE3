#pragma once
#include "Sprite.h"
#include <memory>
#include <array>
#include <string>
#include "GlobalVariables/GlobalVariables.h"

class Camera;

class HPBar
{
public:
	HPBar(const std::string& groupName = "HPBar");

	void Initialize();

	void Update(Camera* camera);

	void Draw();

public:

	void SetParent(const Vector3* parent) { parent_ = parent; }

	void SetMaxHP(float maxHP);

	void SetDamage(float damage);

private:

	void SetGlobalVariable();

	void ApplyGlobalVariable();

private:

	GlobalVariables* globalVariable_;

	std::string groupName_;

	enum EnumV2Info {
		Size,
		Interval,
		Position,
		Offset,
		V2EndCount,
	};

	Vector2 v2Info_[V2EndCount];

	std::string v2ItemNames[V2EndCount] = {
		"サイズ",
		"フレームの空白の幅",
		"固定の場合のポジション",
		"親子付けした場合のローカルのポジション"
	};

private:

	enum SpriteNames {
		kFrame,
		kBack,
		kHP,
		kDamage,
		kSpriteCount,
	};

	std::array<std::unique_ptr<Sprite>, SpriteNames::kSpriteCount> sprites_;

	const Vector3* parent_ = nullptr;

	float maxHP_;

	float HP_;

	float time_;

	float damageSize_;
	float HPSize_;

	bool isDamage_;
};
