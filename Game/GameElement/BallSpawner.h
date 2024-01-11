#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "SoUtils/Math/Angle.h"
#include "SoUtils/SoLib/SoLib.h"

class BallSpawner
{
public:
	BallSpawner();
	~BallSpawner() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	/// @brief 弾の発射
	void Fire();

private:

	Vector3 position_;

	Matrix4x4 transMat_ = Matrix4x4::MakeIdentity4x4();

	float fireAngle_ = 30._deg;

	SoLib::Time::DeltaTimer fireSpanTimer_;

	float fireSpan_ = 1.f;

};