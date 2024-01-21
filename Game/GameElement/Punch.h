#pragma once
#include "IEntity.h"
#include "IWeapon.h"
#include "Vector3.h"
#include "SoUtils/Math/Euler.h"
#include "Shape/Sphere.h"
#include <SoUtils/SoLib/SoLib_Timer.h>

class Punch :public IEntity, public IWeapon {
	enum class HandType {
		kLeft,
		kRight,
	};

public:
	Punch();
	~Punch() = default;

	void Init() override;

	void SetHandType(HandType type);

	void Update(const float deltaTime) override;

	void Draw() override;

	void AttackUpdate(const float deltaTime);

	void Attack(const AttackType attackType = AttackType::kNormal) override;

	bool IsAttacked() const override;

	float GetDamage() const override;

	const Sphere *GetCollision() const;

private:

	/// @brief アフィン行列の計算
	void CalcTransMat();

private:

	HandType handType_;

	uint32_t model_;

	Vector3 origin_;
	Vector3 target_;

	Vector3 scale_;
	SoLib::Math::Euler rotate_;
	Sphere sphere_;

	Matrix4x4 transMat_;

	SoLib::Time::DeltaTimer attackTimer_;
	SoLib::Time::DeltaTimer followTimer_;
private:

	const float kModelScale_ = 2.f;

};