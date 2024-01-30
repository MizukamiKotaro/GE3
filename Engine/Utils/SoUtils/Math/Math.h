#pragma once
#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix4x4.h"
#include "Euler.h"

namespace SoLib {
	namespace Math {

		Matrix4x4 Affine(const Vector3 &scale, const Vector3 &rotate, const Vector3 &transform);
		Matrix4x4 Affine(const Vector3 &scale, const Quaternion &quaternion, const Vector3 &transform);


		Quaternion MakeQuaternion(const SoLib::Math::Euler &euler);

		Vector3 EulerToDirection(const SoLib::Math::Euler &euler);

		SoLib::Math::Euler DirectionToEuler(const Vector3 &direction);

	}
}
