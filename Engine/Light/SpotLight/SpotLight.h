#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>

class SpotLight
{
public:
	struct SpotLightData
	{
		Vector4 color; // ライトの色
		Vector3 position; // 位置
		float intensity; // 輝度
		Vector3 direction; // スポットライトの方向
		float distance; // ライトの届く距離
		float decay; // 減衰率
		float cosAngle; // スポットライトの余弦
		float cosFalloffStart;
		float padding;
	};

	SpotLight();
	~SpotLight();

	void Update();

	const D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return resource_->GetGPUVirtualAddress(); }

public:
	SpotLightData* light_ = nullptr;

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
};
