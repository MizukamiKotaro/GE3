#pragma once
#include <cstdint>

struct Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
};

inline Vector4 operator *(Vector4 &a, const float b) {
	Vector4 result;
	for (uint32_t i = 0u; i < 4u; i++) {
		(&result.x)[i] = (&a.x)[i] * b;
	}
	return result;
}