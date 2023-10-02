#pragma once
#include "Math/Matrix4x4.h"

//VS.hlslにも
struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};