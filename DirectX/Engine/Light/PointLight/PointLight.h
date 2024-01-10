#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>
#include "Matrix4x4.h"
#include "GraphicsPipelines/GraphicsPiplineManager/GraphicsPiplineManager.h"

class Camera;

class PointLight
{
public:
	struct PointLightData
	{
		Vector4 color; // ライトの色
		Vector3 position; // 位置
		float intensity; // 輝度
		float radius; // ライトの届く距離
		float decay; // 減衰率
		float padding[2];
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Matrix4x4 WorldInverse;
	};

	PointLight();
	~PointLight();

	void Update();

	void Draw(const Camera& camera, BlendMode blendMode = BlendMode::kBlendModeNormal);

	const D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return resource_->GetGPUVirtualAddress(); }

private:

	void CreateTransformationResource();

public:
	PointLightData* light_ = nullptr;

private:

	static const GraphicsPiplineManager::PiplineType piplineType = GraphicsPiplineManager::PiplineType::POINT_LIGHT;

	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_;
	TransformationMatrix* transformationData_;

	uint32_t meshHundle_;

	static const Matrix4x4 scaleMat_;

	static const Matrix4x4 scaleInverseMat_;
};
