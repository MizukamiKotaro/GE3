#pragma once
#include "Utils/Math/Vector3.h"
#include "Utils/Math/Matrix4x4.h"
#include <cstdint>

class AABB
{
public:

	void Draw(const Matrix4x4 &viewProjectionMatrix, const Matrix4x4 &viewportMatrix, uint32_t color);

	/// @brief Transform行列の生成
	/// @return transform行列
	Matrix4x4 TransMat() const;

	/// @brief 新規生成
	/// @param origin 原点
	/// @param radius 半径
	/// @return AABB構造体
	static AABB Create(const Vector3 &origin, const Vector3 &radius);


	Vector3 GetCentor() const;
	Vector3 GetRadius() const;

	AABB AddPos(const Vector3 &vec) const;

	AABB Swap() const;



private:


public:
	Vector3 min;
	Vector3 max;
};

