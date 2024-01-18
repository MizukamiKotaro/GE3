#pragma once
#include <stdint.h>
#include "SoLib_ImGui.h"

/// 参考サイト
/// https://easings.net/ja
///

namespace SoLib {

	inline float easeLinear(float number) { return number; }

	float easeInSine(float x);
	float easeOutSine(float x);
	float easeInOutSine(float x);

	float easeInQuad(float x);
	float easeOutQuad(float x);
	float easeInOutQuad(float x);

	float easeInBack(float x);
	float easeOutBack(float x);
	float easeInOutBack(float x);

	float easeInCirc(float x);
	float easeOutCirc(float x);

	float easeInExpo(float x);
	float easeOutExpo(float x);

	float easeInOutQuint(float x);


	float easeOutBounce(float x);

	float easeOutElastic(float x);

	struct EaseFunc {

		using EaseAndString = std::pair<float (*)(float), std::string>;
	public:
		float operator()(float value) const {
			return easeFunc_(value);
		}
		static const std::vector<EaseAndString> &GetEasePair();

		EaseFunc &operator=(const EaseFunc &) = default;
		EaseFunc &operator=(const int32_t index);

		int32_t GetNumber() const;

		//void ImGuiWidget(const char *const label);

		float (*easeFunc_)(float) = easeLinear;
	};

	template<>
	bool ImGuiWidget(const char *label, EaseFunc *value);


	class IEase {
	public:
		virtual float operator()(float value) const = 0;

	private:
		//float (*easeFunc)(float) ;
	};

	class EaseInSine : public IEase { public: float operator()(float value) const override { return easeInSine(value); } };
	class EaseOutSine : public IEase { public: float operator()(float value) const override { return easeOutSine(value); } };
	class EaseInOutSine : public IEase { public: float operator()(float value) const override { return easeInOutSine(value); } };

	/// @brief 色用の線形補間
	/// @param statColor 始点RGBA色
	/// @param endColor 終点RGBA色
	/// @param easingVolume 係数
	/// @return 計算結果
	uint32_t ColorLerp(uint32_t statColor, uint32_t endColor, float easingVolume);

}