#include "PointLight.h"
#include "DirectXCommon/DirectXCommon.h"

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
