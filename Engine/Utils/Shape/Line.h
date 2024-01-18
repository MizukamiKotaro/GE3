#pragma once
#include "Utils/Math/Vector3.h"
#include <algorithm>

struct ILine {
protected:
	/// @brief 最近接点を算出する
	/// @param point 計算する座標
	/// @return 最近接点の比率
	float ClosestProgress(const Vector3 &point) const;

public:

	Vector3 origin; // 始点
	Vector3 diff; // 終点への差分ベクトル
};

struct Line
{
	Vector3 origin; // 始点
	Vector3 diff; // 終点への差分ベクトル
};

struct Ray
{
	Vector3 origin; // 始点
	Vector3 diff; // 終点への差分ベクトル
};

struct Segment :public ILine {

	float ClosestProgress(const Vector3 &point) const { return std::clamp<float>(ILine::ClosestProgress(point), 0.f, 1.f); }

};