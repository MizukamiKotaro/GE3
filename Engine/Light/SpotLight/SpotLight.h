#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>
#include "Matrix4x4.h"
#include "GraphicsPipelines/GraphicsPiplineManager/GraphicsPiplineManager.h"

class Camera;

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

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverse;
	};

	SpotLight();
	~SpotLight();

	void Update();

	void Draw(const Camera& camera, BlendMode blendMode = BlendMode::kBlendModeNormal);

	const D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return resource_->GetGPUVirtualAddress(); }

private:

	void CreateTransformationResource();

public:
	SpotLightData* light_ = nullptr;

	bool isDraw_;

private:

	static const GraphicsPiplineManager::PiplineType piplineType = GraphicsPiplineManager::PiplineType::SPOT_LIGHT;

	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_;
	TransformationMatrix* transformationData_;

	uint32_t meshHundle_;

	static const Matrix4x4 scaleMat_;

	static const Matrix4x4 scaleInverseMat_;
};
