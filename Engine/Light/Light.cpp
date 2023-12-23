#include "Light.h"
#include "DirectXCommon/DirectXCommon.h"

DirectionalLight::DirectionalLight()
{
	//平行光源用のリソースを作る。
	resource_ = DirectXCommon::CreateBufferResource(sizeof(DirectionalLightData));
	//データを書き込む
	light_ = nullptr;
	//書き込むためのアドレスを取得
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&light_));
	//書き込んでいく
	light_->color = { 1.0f,1.0f,1.0f,1.0f };
	light_->direction = { 0.0f,-1.0f,0.0f };
	light_->intensity = 1.0f;
}

DirectionalLight::~DirectionalLight()
{
	resource_->Release();
}

void DirectionalLight::Update()
{
	light_->direction = light_->direction.Normalize();
}

PointLight::PointLight()
{
	resource_ = DirectXCommon::CreateBufferResource(sizeof(PointLightData));
	//データを書き込む
	light_ = nullptr;
	//書き込むためのアドレスを取得
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&light_));
	//書き込んでいく
	light_->color = { 1.0f,1.0f,1.0f,1.0f };
	light_->position = { 0.0f,0.0f,0.0f };
	light_->intensity = 1.0f;
	light_->radius = 1.0f;
	light_->decay = 0.1f;
}

PointLight::~PointLight()
{
	resource_->Release();
}

void PointLight::Update()
{
	if (light_->radius <= 0.0f) {
		light_->radius = 0.01f;
	}
}

LightSingleton* LightSingleton::GetInstance()
{
	static LightSingleton instance;
	return &instance;
}

void LightSingleton::Init()
{
	directionalLight_ = std::make_unique<DirectionalLight>();
	pointLight_ = std::make_unique<PointLight>();
}

void Light::Init()
{
	directionalLight_ = LightSingleton::GetInstance()->GetDirectionaLight();

	pointLight_ = LightSingleton::GetInstance()->GetPointLight();
}
