#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "../Shape/Line.h"
#include "../Shape/Plane.h"
#include "../Shape/Sphere.h"

namespace Calc
{
	enum class LineType {
		Line, // 直線
		Ray, // 半直線
		Segment //線分 
	};


	//適当な垂直なベクトルを求める
	Vector3 Perpendicular(const Vector3 &vector);

	//長さを出す
	float MakeLength(const Vector2 &v);
	float MakeLength(const Vector3 &v);
	float MakeLength(const Vector2 &v1, const Vector2 &v2);
	float MakeLength(const Vector3 &v1, const Vector3 &v2);

	//点v1と線分v1v2の距離
	float MakeLength(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3);
	float MakeLength(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);

	//点v1と線v1v2の距離
	float MakeLength(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, LineType linetype);
	float MakeLength(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, LineType linetype);

	Vector3 Project(const Vector3 &v1, const Vector3 &v2);
	Vector3 ClosestPoint(const Vector3 &point, const Segment &segment);
	Vector3 ClosestPoint(const Vector3 &point, const Ray &ray);
	Vector3 ClosestPoint(const Vector3 &point, const Line &line);

	//今更ながらtを求める
	float MakeT(const Vector3 &point, const Segment &segment);
	float MakeT(const Vector3 &point, const Ray &ray);
	float MakeT(const Vector3 &point, const Line &line);

	//平面との距離
	float MakeLength(const Sphere &sphere, const Plane &plane);
	float MakeLength(const Vector3 &position, const Plane &plane);

	//正規化
	Vector2 Normalize(const Vector2 &v);
	Vector3 Normalize(const Vector3 &v);

	//内積
	float Dot(const Vector2 &v1, const Vector2 &v2);
	// 内積
	float Dot(const Vector3 &v1, const Vector3 &v2);

	/// @brief 内積
	/// @tparam I 要素数
	/// @param v1 配列A
	/// @param v2 配列B
	/// @return ドット積
	template<uint32_t I>
	float Dot(const float *v1, const float *v2);

	//外積
	float Outer(const Vector2 &v1, const Vector2 &v2);

	//クロス積
	Vector3 Cross(const Vector3 &v1, const Vector3 &v2);

	//線形補間
	Vector3 Lerp(const Vector3 &p0, const Vector3 &p1, float t);
	//ベジエ
	Vector3 Bezier(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, float t);
	//キャトムルロム
	Vector3 Catmull_rom(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, const Vector3 &p3, float t);

}


template<uint32_t I>
float Calc::Dot(const float *v1, const float *v2) {
	float result{};
	for (uint32_t i = 0u; i < I; i++) {
		result += v1[i] * v2[i];
	}
	return result;
}
