#pragma once
#include <Vector3.h>
#include <Matrix4x4.h>
#include <SoUtils/Math/Euler.h>
#include <SoUtils/Graphics/Color.h>
#include "IEntity.h"
#include "IWeapon.h"

class Stage;

class Sword :public IEntity, public IWeapon {
public:
	Sword();
	~Sword() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	void SetPos(const Vector3 &pos) { position_ = pos; }

	void Attack() override {}

	bool IsAttacked() const override { return false; }

	const Vector3 &GetPos() const { return position_; }

	void CalcTransMat();

private:

	uint32_t model_;

	SoLib::Color::RGB4 color_ = 0xFFFFFFFF;

	Vector3 scale_ = Vector3::one;
	SoLib::Math::Euler rotate_;
	Vector3 position_;

	Matrix4x4 transMat_;



};