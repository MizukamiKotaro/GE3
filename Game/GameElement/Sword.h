#pragma once
#include <Vector3.h>
#include <Matrix4x4.h>
#include <SoUtils/Math/Euler.h>
#include <SoUtils/Graphics/Color.h>
#include <SoUtils/SoLib/SoLib_Timer.h>
#include "IEntity.h"
#include "IWeapon.h"
#include "SoUtils/SoLib/SoLib.h"
#include "Shape/Capsule.h"

class Stage;

class Sword :public IEntity, public IWeapon {
public:
	Sword();
	~Sword() = default;

	void Init();

	void Update(const float deltaTime);

	void AttackUpdate(const float deltaTime);

	void Draw();

	void SetPos(const Vector3 &pos) { position_ = pos; }

	void Attack(const AttackType attackType = AttackType::kNormal) override;

	bool IsAttacked() const override { return attackTimer_.IsActive(); }

	const Capsule *GetCollision() const;

	const Vector3 &GetPos() const { return position_; }

	const float &GetRotateZ() const { return rotate_.z; }

	float GetDamage() const override;

	void CalcTransMat();

	float GetBeforeRotate() const { return beforeRotate_; }

private:

	Capsule capsule_;

	uint32_t model_;

	SoLib::Color::RGB4 color_ = 0xFFFFFFFF;

	Vector3 scale_ = Vector3::one;
	SoLib::Math::Euler rotate_;
	Vector3 position_;

	Matrix4x4 transMat_;

	float beforeRotate_;

	SoLib::Time::DeltaTimer attackTimer_;

	//SoLib::EaseFunc attackEasing_{ SoLib::easeInOutQuad };

	float targetAngle_ = 360._deg;

};