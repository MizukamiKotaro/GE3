#include "Light.h"

Light::~Light()
{
	resource_->Release();
}
