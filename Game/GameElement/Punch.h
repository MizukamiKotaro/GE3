#pragma once
#include "IEntity.h"
#include "IWeapon.h"
#include "Vector3.h"
#include "SoUtils/Math/Euler.h"
#include "Shape/Sphere.h"

class Punch :public IEntity, public IWeapon {
public:
	Punch();
	~Punch() = default;

	void Init() override;

	void Update(const float deltaTime) override;

	void Draw() override;

	void Attack() override;

	bool IsAttacked() const override;

	float GetDamage() const override;
private:

	/// @brief アフィン行列の計算
	void CalcTransMat();

private:

	Vector3 scale_;
	SoLib::Math::Euler rotate_;
	Sphere sphere_;

	Matrix4x4 transMat_;

};