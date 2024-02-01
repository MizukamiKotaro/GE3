#include "Math.h"

Matrix4x4 SoLib::Math::Affine(const Vector3 &scale, const Vector3 &rotate, const Vector3 &transform) {
	return Matrix4x4::MakeAffinMatrix(scale, rotate, transform);
}

Matrix4x4 SoLib::Math::Affine(const Vector3 &scale, const Quaternion &quaternion, const Vector3 &transform) {
	Matrix4x4 result;

	result = Matrix4x4::MakeRotateMatrix(quaternion);

	Vector4 *const matItr = reinterpret_cast<Vector4 *>(*result.m);
	for (uint8_t i = 0u; i < 3u; i++) {
		matItr[i] = matItr[i] * (&scale.x)[i];
	}
	*reinterpret_cast<Vector3 *>(result.m[3]) = transform;

	return result;
}

Vector3 SoLib::Math::EulerToDirection(const SoLib::Math::Euler &euler) {
	Vector3 direction;
	enum SinCos {
		kSin,
		kCos
	};

	const std::array<float, 2u> eX = { std::sin(euler.x), std::cos(euler.x) };
	const std::array<float, 2u> eY = { std::sin(euler.y), std::cos(euler.y) };
	const std::array<float, 2u> eZ = { std::sin(euler.z), std::cos(euler.z) };

	direction.x = (eX[kCos] * eY[kSin] * eZ[kCos] + eX[kSin] * eZ[kSin]);
	direction.y = (eX[kCos] * eY[kSin] * eZ[kSin] - eX[kSin] * eZ[kCos]);
	direction.z = (eX[kCos] * eY[kCos]);
	return direction;
}
