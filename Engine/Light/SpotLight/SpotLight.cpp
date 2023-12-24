#include "SpotLight.h"
#include "DirectXCommon/DirectXCommon.h"
#include <algorithm>

SpotLight::SpotLight()
{
	resource_ = DirectXCommon::CreateBufferResource(sizeof(SpotLightData));
	//データを書き込む
	light_ = nullptr;
	//書き込むためのアドレスを取得
	resource_->Map(0, nullptr, reinterpret_cast<void**>(&light_));
	//書き込んでいく
	light_->color = { 1.0f,1.0f,1.0f,1.0f };
	light_->position = { 0.0f,0.0f,0.0f };
	light_->intensity = 1.0f;
	light_->direction = { 0.0f,-1.0f,0.0f };
	light_->distance = 5.0f;
	light_->decay = 0.1f;
	light_->cosAngle = 0.7f;
	light_->cosFalloffStart = 0.8f;
}

SpotLight::~SpotLight()
{
	resource_->Release();
}

void SpotLight::Update()
{
	light_->direction = light_->direction.Normalize();

	light_->cosAngle = std::clamp<float>(light_->cosAngle, -1.0f, 0.998f);
	light_->cosFalloffStart = std::clamp<float>(light_->cosFalloffStart, light_->cosAngle + 0.001f, 0.999f);

	if (light_->distance <= 0.0f) {
		light_->distance = 0.01f;
	}
}
