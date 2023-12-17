#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include <wrl.h>
#include <d3d12.h>

class Light {
public:

	enum class LightType {
		kNone,
		kDirectional,
		kCountLightType,
	};

	~Light();

	static Light* CreateLight();

	struct DirectionalLight
	{
		Vector4 color; // ライトの色
		Vector3 direction; // ライトの向き
		float intensity; // 輝度
	};


private:
	Light() = default;

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
	DirectionalLight* directionalLightData_;

};
