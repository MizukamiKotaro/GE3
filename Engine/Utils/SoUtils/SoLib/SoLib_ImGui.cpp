#include "SoLib_ImGui.h"
#include "../../Externals/imgui/imgui_internal.h"

template<>
void SoLib::ImGuiText([[maybe_unused]] const char *const label, [[maybe_unused]] const std::string &text) {
#ifdef _DEBUG
	ImGui::Text("%s : %s", label, text.c_str());
#endif // _DEBUG
}

template<>
bool SoLib::ImGuiWidget<bool>([[maybe_unused]] const char *const label, [[maybe_unused]] bool *const value) {
#ifdef _DEBUG
	return ImGui::Checkbox(label, value);

#else
	return false;
#endif // _DEBUG

}

template<>
bool SoLib::ImGuiWidget<int32_t>([[maybe_unused]] const char *const label, [[maybe_unused]] int32_t *const value) {
#ifdef _DEBUG
	return ImGui::InputInt(label, value);
#else
	return false;
#endif // _DEBUG
}

template<>
bool SoLib::ImGuiWidget<float>([[maybe_unused]] const char *const label, [[maybe_unused]] float *const value) {
#ifdef _DEBUG
	return ImGui::DragFloat(label, value, 0.1f);
#else
	return false;
#endif // _DEBUG
}

template<>
bool SoLib::ImGuiWidget<double>([[maybe_unused]] const char *const label, [[maybe_unused]] double *const value) {
#ifdef _DEBUG
	return ImGui::InputDouble(label, value, 0.1f);
#else
	return false;
#endif // _DEBUG
}

template<>
bool SoLib::ImGuiWidget<Vector2>([[maybe_unused]] const char *const label, [[maybe_unused]] Vector2 *const value) {
#ifdef _DEBUG
	return ImGui::DragFloat2(label, &value->x, 0.1f);
#else
	return false;
#endif // _DEBUG
}

template<>
bool SoLib::ImGuiWidget<Vector3>([[maybe_unused]] const char *const label, [[maybe_unused]] Vector3 *const value) {
#ifdef _DEBUG
	return ImGui::DragFloat3(label, &value->x, 0.1f);
#else
	return false;
#endif // _DEBUG
}

template<>
bool SoLib::ImGuiWidget<Vector4>([[maybe_unused]] const char *const label, [[maybe_unused]] Vector4 *const value) {
#ifdef _DEBUG
	return ImGui::DragFloat4(label, &value->x, 0.1f);
#else
	return false;
#endif // _DEBUG
}

template<>
bool SoLib::ImGuiWidget([[maybe_unused]] const char *const label, [[maybe_unused]] std::string *const value) {
#ifdef _DEBUG
	return ImGui::InputText(label, value->data(), value->size() + 1);
#else
	return false;
#endif // _DEBUG
}

template<>
bool SoLib::ImGuiWidget([[maybe_unused]] const char *const label, [[maybe_unused]] SoLib::Math::Angle::Radian *const value) {
#ifdef _DEBUG
	return ImGui::SliderAngle(label, &value->Get(), 0.1f);
#else
	return false;
#endif // _DEBUG
}

bool SoLib::ImGuiWidgetAngle([[maybe_unused]] const char *const label, [[maybe_unused]] float *const value, [[maybe_unused]] float min, [[maybe_unused]] float max) {
#ifdef _DEBUG
	return ImGui::SliderAngle(label, value, min, max);
#else
	return false;
#endif // _DEBUG
}

bool SoLib::ImGuiSliderEuler(const char *const label, float *value, float min, float max)
{
	ImGuiWindow *window = ImGui::GetCurrentWindow();
	if (window->SkipItems) { return false; }

	ImGuiContext &g = *GImGui;
	bool value_changed = false;
	ImGui::BeginGroup();
	ImGui::PushID(label);
	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	for (int i = 0; i < 3; i++) {
		ImGui::PushID(i);
		if (i > 0) { ImGui::SameLine(0, g.Style.ItemInnerSpacing.x); }
		value_changed |= ImGui::SliderAngle("", &value[i], min, max);
		ImGui::PopID();
		ImGui::PopItemWidth();
	}
	ImGui::PopID();

	const char *label_end = ImGui::FindRenderedTextEnd(label);
	if (label != label_end) {
		ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
		ImGui::TextEx(label, label_end);
	}

	ImGui::EndGroup();
	return value_changed;
}
