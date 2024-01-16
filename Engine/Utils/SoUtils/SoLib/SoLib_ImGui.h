#pragma once
#include <stdint.h>
#include "../../Externals/imgui/imgui.h"

#include "SoLib_Traits.h"	// 型情報など

#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"

#include "../Math/Angle.h"
#include "SoUtils/IO/VariantItem.h"

#include <string>

namespace SoLib {

	// ImGuiの前方宣言
	template<IsNotPointer T>
	bool ImGuiWidget(const char *const label, T *const value) {
		// ImGuiWidget関数を呼び出す
		return value->ImGuiWidget(label);
	}

	template<IsNotPointer T>
	void ImGuiText(const char *const label, const T &text);

	template<>
	void ImGuiText(const char *const label, const std::string &text);

	template <>
	bool ImGuiWidget<bool>(const char *const label, bool *const value);

	template <>
	bool ImGuiWidget<int32_t>(const char *const label, int32_t *const value);
	template <>
	bool ImGuiWidget<float>(const char *const label, float *const value);
	template <>
	bool ImGuiWidget<double>(const char *const label, double *const value);
	template <>
	bool ImGuiWidget<Vector2>(const char *const label, Vector2 *const value);
	template <>
	bool ImGuiWidget<Vector3>(const char *const label, Vector3 *const value);
	template <>
	bool ImGuiWidget<Vector4>(const char *const label, Vector4 *const value);

	template <>
	bool ImGuiWidget<std::string>(const char *const label, std::string *const value);

	template<SoLib::IsNotPointer T>
	bool ImGuiWidget(VariantItem<T> *const item);
	template <>
	bool ImGuiWidget<SoLib::Math::Angle::Radian>(const char *const label, SoLib::Math::Angle::Radian *const value);

	bool ImGuiWidgetAngle(const char *const label, float *const value, float min = -360.f, float max = +360.f);

	/*template<typename T>
	inline bool ImGuiWidget(VariantItem<T> *const value);*/

	bool ImGuiSliderEuler(const char *const label, float *value, float min = -180.f, float max = 180.f);

}

#pragma region inline関数の記述

template<SoLib::IsNotPointer T>
bool SoLib::ImGuiWidget(VariantItem<T> *const value) { return SoLib::ImGuiWidget(value->GetKey().c_str(), &value->GetItem()); }

#pragma endregion
