#include "PointLight.hlsli"

SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

struct PointLight {
	float32_t4 color;
	float32_t3 position;
	float32_t intensity;
	float32_t radius;
	float32_t decay;
};
ConstantBuffer<PointLight> gPointLight : register(b0);

struct Camera {
	float32_t3 worldPosition;
};
ConstantBuffer<Camera> gCamera : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

	// pointLight
	float32_t3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
	float32_t NdotL = dot(normalize(input.normal), -pointLightDirection);
	float32_t cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
	float32_t3 halfVector = normalize(-pointLightDirection + toEye);
	float32_t NdotH = dot(normalize(input.normal), halfVector);
	float32_t specularPow = pow(saturate(NdotH), 40.0f);

	float32_t distance = length(gPointLight.position - input.worldPosition);
	float32_t factor = pow(saturate(-distance / gPointLight.radius * 5.0f + 1.0f), gPointLight.decay);

	float32_t3 diffusePL = gPointLight.color.rgb * cos * gPointLight.intensity * factor;

	float32_t3 specularPL = gPointLight.color.rgb * gPointLight.intensity * specularPow * factor;

	output.color.rgb = diffusePL + specularPL;
	output.color.a = (output.color.r + output.color.g + output.color.b) / 3.0f * 0.6f;

	if (output.color.a == 0) {
		discard;
	}
	
	return output;
}