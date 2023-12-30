#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"
#include "Utils/Transform/Transform.h"
#include "GraphicsPipelines/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include <optional>
#include <list>
#include "Utils/Shape/AABB.h"
#include "Light/Light.h"

class Camera;

class Particle
{
public:

	static const uint32_t kNumInstance = 256;

	Particle(const std::string& fileName);
	Particle(uint32_t textureHundle);
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

	struct Active {
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};

	struct Emitter
	{
		Vector3 pos;
		Vector3 min;
		Vector3 max;
		uint32_t generateParticleNum_; // 一回に生成する数
		float generateCoolTime_; // 生成の間隔
		float countTime_; // 生成をするための時間のカウント
	};

	struct AccelerationField
	{
		Vector3 acceleration;
		AABB area;
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void Update();

	void Draw(const Camera& camera, BlendMode blendMode = BlendMode::kBlendModeNormal);

	static void PreDraw() { GraphicsPiplineManager::GetInstance()->PreDraw(piplineType); }

public:

	enum class BillboardType
	{
		X,
		Y,
		Z,
		ALL
	};

	void SetBillboardType(BillboardType type) { billboardTypeOpt_ = type; }

	void ClearBillboardType() { billboardTypeOpt_ = std::nullopt; }

	void GenerateParticle();

	void SetDirectionalLight(const DirectionalLight* light) { light_.SetDirectionalLight(light); }

	void SetPointLight(const PointLight* light) { light_.SetPointLight(light); }

	void SetSpotLight(const SpotLight* light) { light_.SetSpotLight(light); }

private:

	void CreateSRV();

	void CreateResources();

	void CreateMaterialResource();

	void CreateInstancingResource();

	void InitVariables();

	Active CreateActive();

private:
	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> instancingResource_;
	ParticleForGPU* instancingData_;

public:

	std::list<Active> actives_;

	Emitter emitter;

private:

	static const GraphicsPiplineManager::PiplineType piplineType = GraphicsPiplineManager::PiplineType::PARTICLE;

	Light light_;

	std::optional<BillboardType> billboardTypeOpt_ = BillboardType::Y;
	BillboardType billbordType = BillboardType::Y;

	Matrix4x4 uvMatrix_;

	Vector3 uvScale_;
	Vector3 uvRotate_;
	Vector3 uvPos_;

	uint32_t meshHundle_;

	uint32_t textureHundle_;

	D3D12_CPU_DESCRIPTOR_HANDLE srvCPUDescriptorHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle_;

};

