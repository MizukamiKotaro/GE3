#pragma once

class IWeapon {

	/// @brief 攻撃発生
	virtual void Attack() = 0;

	/// @brief 攻撃中か
	/// @return 攻撃中
	virtual bool IsAttacked() = 0;
};