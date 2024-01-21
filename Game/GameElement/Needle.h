#pragma once
#include "IEntity.h"
#include "IWeapon.h"
#include "Vector3.h"
#include "SoUtils/Math/Euler.h"
#include "SoUtils/SoLib/SoLib.h"
#include "Shape/Capsule.h"

class Needle : public IEntity, public IWeapon {
public:
	Needle();
	~Needle() = default;

	void Init() override;

	void SetParametor();

	void Update(const float deltaTime) override;

	void Draw() override;

	void AttackUpdate(const float deltaTime);

	void Attack(const AttackType attackType = AttackType::kNormal) override;

	bool IsAttacked() const override;

	float GetDamage() const override;

	const Capsule *GetCollision() const;

private:

	void CalcSegment();

	void CalcTransMat();

private:

	uint32_t model_;

	SoLib::Time::DeltaTimer attackTimer_;
	SoLib::Time::DeltaTimer followTimer_;

	Vector3 scale_;
	SoLib::Math::Euler rotate_;
	Vector3 translate_;

	Matrix4x4 transMat_;

	Capsule capsule_;

};