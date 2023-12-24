#include "SoLib_Lerp.h"

#include "../Math/Vector3.h"
#include "../Math/calc.h"
#include <algorithm>
#include <cassert>
#include <cmath>

namespace SoLib {


	Vector3 Slerp(const Vector3 &start, const Vector3 &end, float t) {
		if (start.Length() == 0.f || end.Length() == 0.f) // 片方の長さが0であった場合、「角度」が存在しないので通常の線形補間を行う。
			return Lerp(start, end, t);

		Vector3 nStart = start.Normalize();
		Vector3 nEnd = end.Normalize();

		float angle = Calc::Dot(nStart, nEnd);

		angle = std::clamp(angle, -1.f, 1.f);	// 計算上の誤差を-1.f~1.fの間に丸める
		const float theta = std::acos(angle);	// 丸めないとnan(-ind)が返ってエラーが発生する。StartとEndの間の角度
		const float LerpLength = Lerp(start.Length(), end.Length(), t);	// ベクトル長のみの線形補間

		const float sinTheta = std::sin(theta);	// 正規化後においてのEndに投影したStartベクトルの高さ

		if (sinTheta == 0.f)	// ゼロ除算対策。 
			return LerpLength * nStart; // nStartとnEndが同じ == 角度の差がない == 距離だけの線形補間でよい。
		return LerpLength * (std::sin((1 - t) * theta) * nStart + std::sin(t * theta) * nEnd) /
			std::sin(theta);
	}
}