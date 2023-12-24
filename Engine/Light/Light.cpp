#include "Light.h"

LightSingleton* LightSingleton::GetInstance()
{
	static LightSingleton instance;
	return &instance;
}

void LightSingleton::Init()
{
	directionalLight_ = std::make_unique<DirectionalLight>();
	pointLight_ = std::make_unique<PointLight>();
	spotLight_ = std::make_unique<SpotLight>();
}

void Light::Init()
{
	LightSingleton* light = LightSingleton::GetInstance();

	directionalLight_ = light->GetDirectionaLight();

	pointLight_ = light->GetPointLight();

	spotLight_ = light->GetSpotLight();
}
