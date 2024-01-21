#pragma once
#include "Utils/Math/Vector3.h"
#include "Utils/Math/Matrix4x4.h"
#include <cstdint>

struct Sphere
{
public:
	/*Sphere(const Vector3& center, const float& radius);

	Sphere(const Vector3& center, const Vector3& rotate, const float& radius);

	Sphere(const Vector3& center, const Vector3& scale, const Vector3& rotate, const float& radius);*/

	void Initialize(const Vector3 &center, const float &radius);
	//void Initialize(const Vector3& center, const Vector3& rotate, const float& radius);
	//void Initialize(const Vector3& center, const Vector3& scale, const Vector3& rotate, const float& radius);

	void Draw(const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);

	/// @brief アフィン行列の計算
	/// @return アフィン行列
	Matrix4x4 MakeAffineMatrix() const;

	static Matrix4x4 MakeAffineMatrix(const Vector3 &center, const float radius);

private:


public:

	Vector3 center_;
	/*Vector3 scale_;
	Vector3 rotate_;*/
	float radius_;
};

