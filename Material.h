#pragma once
#include "Math/Vector4.h"
#include <stdint.h>
#include "Math/Matrix4x4.h"

//PS.hlslにも
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};