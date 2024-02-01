#pragma once
#include "Vector3.h"
#include "Vector3Norm.h"

namespace SoLib::Math {

	struct Euler;
}

struct Quaternion final {
	Quaternion() = default;
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
	Quaternion(const Vector3 &vec, float w = 0.f) :w(w) { this->vec() = vec; };

	float x, y, z;
	float w;

	/// @brief ベクトル部を取得する関数
	/// @return クォータニオンのベクトル部 [ 参照 ]
	inline Vector3 &vec() noexcept { return *reinterpret_cast<Vector3 *>(&x); }
	inline const Vector3 &vec() const noexcept { return *reinterpret_cast<const Vector3 *>(&x); }

	/// @brief 単位クォータニオン
	static const Quaternion Identity;

	/// @brief 共役クォータニオン関数
	/// @return 共役クォータニオン
	inline Quaternion Conjugation() const;

	/// @brief 逆クォータニオン関数
	/// @return 逆クォータニオン
	inline Quaternion Inverse() const;

	/// @brief 二乗ノルム
	/// @return 二乗したクォータニオンの長さ
	inline float LengthSQ() const;

	/// @brief ノルム
	/// @return クォータニオンの長さ
	inline float Length() const;

	/// @brief 正規化関数
	/// @return 単位クォータニオン
	inline Quaternion Normalize() const;

	static inline Vector3 RotateVector(const Vector3 &a, const Quaternion &b);
	inline Vector3 RotateVector(const Vector3 &v) const;

	static Quaternion AnyAxisRotation(const Vector3Norm &axis, float angle);

	static Quaternion Slerp(Quaternion start, const Quaternion &end, float t);

	/// @brief 指定の方向に向く回転クォータニオンを作成
	/// @param direction 向きたい方向( 非正規化 OK )
	/// @return クォータニオン
	static Quaternion LookAt(const Vector3Norm &direction);

};