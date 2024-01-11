#pragma once

#include "DirectionalLight/DirectionalLight.h"
#include "PointLight/PointLight.h"
#include "SpotLight/SpotLight.h"

// モデルなどを生成したときに最初に入れる用
class LightSingleton
{
public:

	static LightSingleton* GetInstance();

	void Init();

	const DirectionalLight* GetDirectionaLight() const { return directionalLight_.get(); }

	const PointLight* GetPointLight() const { return pointLight_.get(); }

	const SpotLight* GetSpotLight() const { return spotLight_.get(); }

private:
	LightSingleton() = default;
	~LightSingleton() = default;
	LightSingleton(const LightSingleton&) = delete;
	LightSingleton& operator=(const LightSingleton&) = delete;

private:
	std::unique_ptr<DirectionalLight> directionalLight_;

	std::unique_ptr<PointLight> pointLight_;

	std::unique_ptr<SpotLight> spotLight_;
};

// モデルなどの変数に使う用
class Light 
{
public:
	void Init();

	void SetDirectionalLight(const DirectionalLight* light) { directionalLight_ = light; }

	void SetPointLight(const PointLight* light) { pointLight_ = light; }

	void SetSpotLight(const SpotLight* light) { spotLight_ = light; }

	const D3D12_GPU_VIRTUAL_ADDRESS GetDirectionalLightGPUVirtualAddress() const { return directionalLight_->GetGPUVirtualAddress(); }

	const D3D12_GPU_VIRTUAL_ADDRESS GetPointLightGPUVirtualAddress() const { return pointLight_->GetGPUVirtualAddress(); }

	const D3D12_GPU_VIRTUAL_ADDRESS GetSpotLightGPUVirtualAddress() const { return spotLight_->GetGPUVirtualAddress(); }

private:

	const DirectionalLight* directionalLight_ = nullptr;

	const PointLight* pointLight_ = nullptr;

	const SpotLight* spotLight_ = nullptr;
};
