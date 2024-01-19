#pragma once
#include <cstdint>

class IWeapon {
public:
	/// @brief 攻撃発生
	virtual void Attack() = 0;

	/// @brief 攻撃中か
	/// @return 攻撃中
	virtual bool IsAttacked() const = 0;

	/// @brief 攻撃ダメージ
	/// @return ダメージ
	virtual float GetDamage() const = 0;

	uint32_t GetNumber() const { return holeNumber_; }

protected:

	uint32_t holeNumber_ = 0u;
};