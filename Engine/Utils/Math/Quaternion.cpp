#include "Quaternion.h"
#include <array>
#include <cmath>
#include <SoUtils/SoLib/SoLib_Lerp.h>
#include "calc.h"

Quaternion const Quaternion::Identity{ Vector3::zero, 1.f };



Quaternion operator+(const Quaternion &a, const Quaternion &b) {
	Quaternion result;

	for (uint32_t i = 0u; i < 4u; i++) {
		(&result.x)[i] = (&a.x)[i] + (&b.x)[i];
	}

	return result;
}

Quaternion operator-(const Quaternion &a, const Quaternion &b) {
	Quaternion result;

	for (uint32_t i = 0u; i < 4u; i++) {
		(&result.x)[i] = (&a.x)[i] - (&b.x)[i];
	}

	return result;
}

Quaternion operator*(const Quaternion &a, const float b) {
	Quaternion result;

	for (uint32_t i = 0u; i < 4u; i++) {
		(&result.x)[i] = (&a.x)[i] * b;
	}

	return result;
}

Quaternion operator/(const Quaternion &a, const float b) {
	Quaternion result;

	for (uint32_t i = 0u; i < 4u; i++) {
		(&result.x)[i] = (&a.x)[i] / b;
	}

	return result;
}

Quaternion operator*(const Quaternion &a, const Quaternion &b) {
	Quaternion result;

	// ベクトル部の算出 ( 実数部は Vector3への代入時に切り捨て )
	result.vec() = Calc::Cross(a.vec(), b.vec()) + a.vec() * b.w + b.vec() * a.w;
	// 実数部の算出 ( ドット積で 実数部を切り捨て )
	result.w = a.w * b.w - Calc::Dot(a.vec(), b.vec());

	return result;
}

Quaternion &operator*=(Quaternion &a, const Quaternion &b) {
	return a = a * b;;
}

Quaternion Quaternion::Conjugation() const {
	Quaternion result = *this;

	// 符号反転
	result.vec() *= -1.f;

	return result;
}

Quaternion Quaternion::Inverse() const {
	const float lengthSQ = this->LengthSQ();
	if (lengthSQ == 0.f) { return *this; }

	return this->Conjugation() / lengthSQ;
}

Quaternion Quaternion::Normalize() const {
	// 長さ
	float length = this->Length();
	// もし長さが0ならそのまま返す( 0なので )
	if (length == 0.f) { return *this; }

	return *this / length;
}

Quaternion Quaternion::AnyAxisRotation(const Vector3Norm &axis, float angle) {
	float halfAngle = angle / 2.f;
	return Quaternion{ axis * std::sin(halfAngle), std::cos(halfAngle) };
}


float Quaternion::LengthSQ() const {
	return Calc::Dot<4u>(&this->x, &this->x);
}

float Quaternion::Length() const {

	return std::sqrt(LengthSQ());
}

Vector3 Quaternion::RotateVector(const Vector3 &a, const Quaternion &b) {

	return b.RotateVector(a);
}

Vector3 Quaternion::RotateVector(const Vector3 &v) const {
	Quaternion result = *this * Quaternion{ v } *this->Conjugation();

	return result.vec();
}

Quaternion Quaternion::Slerp(Quaternion start, const Quaternion &end, float t) {

	// 返り値
	Quaternion result;
	// クォータニオン同士のドット積
	float dot = Calc::Dot<4u>(&start.x, &end.x);

	// 逆方向なら正方向に反転
	if (dot < 0) {
		start = start * -1.f;
		dot = -dot;
	}
	// 同方向のクォータニオンである場合は単純な線形補間を行う。
	if (dot >= 1.f - std::numeric_limits<float>::epsilon()) {
		result = SoLib::Lerp(start, end, t);
		return result;
	}
	// 度数を生成
	const float theta = std::acos(dot);
	// sin値
	const float sin = std::sin(theta);
	// 計算
	result = start * (std::sin((1 - t) * theta) / sin) + end * std::sin(t * theta) / sin;
	return result;
}

Quaternion Quaternion::LookAt(const Vector3Norm &direction) {
	return DirectionToDirection(Vector3::front, direction);
}

Quaternion Quaternion::DirectionToDirection(const Vector3Norm &begin, const Vector3Norm &end) {
	if (not begin) {
		return Quaternion::Identity;
	}
	if (not end) {
		return LookAt(begin);
	}

	// ドット積を計算
	float dot = Calc::Dot(begin, end);
	// acosの時に範囲を超えないように
	dot = std::clamp(dot, -1.f, 1.f);
	// 角度を取得
	const float theta = std::acos(dot);

	// 回転軸のベクトル
	const Vector3Norm cross = Calc::Cross(begin, end);
	// 任意軸回転クォータニオンを作成
	return Quaternion::AnyAxisRotation(cross, theta).Normalize();
}
