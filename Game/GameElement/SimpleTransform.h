#pragma once
#include "Vector3.h"
#include "SoUtils/Math/Euler.h"
#include "Matrix4x4.h"

struct SimpleTransform {
	Vector3 scale_;
	SoLib::Math::Euler rotate_;
	Vector3 translate_;

	Matrix4x4 worldMat_;

	/// @brief 行列の計算
	void CalcTransMat();

	const Vector3 &GetGrobalPos() const { return *reinterpret_cast<const Vector3 *>(worldMat_.m[3]); }
};