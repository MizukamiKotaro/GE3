#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"
#include "Utils/Transform/Transform.h"
#include "ModelCommon/ModelCommon.h"

class Particle
{
public:

	static const uint32_t kNumInstance = 10;

	Particle(const std::string& fileName);
	~Particle();

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};

	struct DirectionalLight
	{
		Vector4 color; // ライトの色
		Vector3 direction; // ライトの向き
		float intensity; // 輝度
	};

	struct Active {
		Transform transform;
		Vector3 velocity;
		Vector4 color;
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

public:

	//void LoadTexture(const std::string& filePath);

private:

	void CreateSRV();

private:
	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> instancingResource_;
	ParticleForGPU* instancingData_;

	ComPtr<ID3D12Resource> directionalLightResource_;
	DirectionalLight* directionalLightData_;


public:

	Active actives_[kNumInstance];

private:
	Matrix4x4 uvMatrix_;

	Vector3 uvScale_;
	Vector3 uvRotate_;
	Vector3 uvPos_;

	uint32_t meshHundle_;

	uint32_t textureHundle_;

	D3D12_CPU_DESCRIPTOR_HANDLE srvCPUDescriptorHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle_;

};

