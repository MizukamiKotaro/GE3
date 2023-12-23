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

	PointLight();
	~PointLight();

	void Update();

	const D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return resource_->GetGPUVirtualAddress(); }

public:
	PointLightData* light_ = nullptr;

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
};

// モデルなどを生成したときに最初に入れる用
class LightSingleton
{
public:

	static LightSingleton* GetInstance();

	void Init();

	const DirectionalLight* GetDirectionaLight() const { return directionalLight_.get(); }

	const PointLight* GetPointLight() const { return pointLight_.get(); }

private:
	LightSingleton() = default;
	~LightSingleton() = default;
	LightSingleton(const LightSingleton&) = delete;
	LightSingleton& operator=(const LightSingleton&) = delete;

private:
	std::unique_ptr<DirectionalLight> directionalLight_;

	std::unique_ptr<PointLight> pointLight_;
};

// モデルなどの変数に使う用
class Light 
{
public:
	void Init();

	void SetDirectionalLight(const DirectionalLight* light) { directionalLight_ = light; }

	void SetPointLight(const PointLight* light) { pointLight_ = light; }

	const D3D12_GPU_VIRTUAL_ADDRESS GetDirectionalLightGPUVirtualAddress() const { return directionalLight_->GetGPUVirtualAddress(); }

	const D3D12_GPU_VIRTUAL_ADDRESS GetPointLightGPUVirtualAddress() const { return pointLight_->GetGPUVirtualAddress(); }

private:

	const DirectionalLight* directionalLight_ = nullptr;

	const PointLight* pointLight_ = nullptr;
};
