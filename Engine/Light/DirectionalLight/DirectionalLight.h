#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>

class DirectionalLight
{
public:
	struct DirectionalLightData
	{
		Vector4 color; // ライトの色
		Vector3 direction; // ライトの向き
		float intensity; // 輝度
	};

	DirectionalLight();
	~DirectionalLight();

	void Update();

	const D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return resource_->GetGPUVirtualAddress(); }

public:
	DirectionalLightData* light_ = nullptr;

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
};
