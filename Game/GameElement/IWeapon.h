#pragma once
#include <cstdint>

class IWeapon {
public:

	enum class AttackType {
		kWeak,		// 弱攻撃
		kNormal,	// 中攻撃
		kStrong,	// 強攻撃
	};

	/// @brief 攻撃発生
	virtual void Attack(const AttackType attackType = AttackType::kNormal) = 0;

	/// @brief 攻撃中か
	/// @return 攻撃中
	virtual bool IsAttacked() const = 0;

	/// @brief 攻撃ダメージ
	/// @return ダメージ
	virtual float GetDamage() const = 0;

	uint32_t GetNumber() const { return holeNumber_; }
	void SetNumber(const uint32_t number) { holeNumber_ = number; }

	bool ImGuiWidget(const char *const label);

protected:

	uint32_t holeNumber_ = 0u;
};