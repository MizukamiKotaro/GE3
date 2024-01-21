#include "Line.h"
#include <calc.h>
#include <cmath>

float ILine::ClosestProgress(const Vector3 &point) const {
	return Calc::Dot((point - origin), diff) / std::powf(diff.Length(), 2);
}

Vector3 ILine::GetPoint(const float t) const {
	return origin + diff * t;
}
