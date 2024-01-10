#include "SpotLight.hlsli"

SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

struct SpotLight {
	float32_t4 color;
	float32_t3 position;
	float32_t intensity;
	float32_t3 direction;
	float32_t distance;
	float32_t decay;
	float32_t cosAngle;
	float32_t cosFalloffStart;
};
ConstantBuffer<SpotLight> gSpotLight : register(b0);

struct Camera {
	float32_t3 worldPosition;
};
ConstantBuffer<Camera> gCamera : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

	// spotLight
	float32_t3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
	float32_t cosAngle = dot(spotLightDirectionOnSurface, gSpotLight.direction);
	float32_t falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));

	float32_t NdotL = dot(normalize(input.normal), -spotLightDirectionOnSurface);
	float32_t cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
	float32_t3 halfVector = normalize(-spotLightDirectionOnSurface + toEye);
	float32_t NdotH = dot(normalize(input.normal), halfVector);
	float32_t specularPow = pow(saturate(NdotH), 40.0f);

	float32_t distance = length(gSpotLight.position - input.worldPosition);
	float32_t factor = pow(saturate(-distance / gSpotLight.distance + 1.0f), gSpotLight.decay);

	float32_t3 diffuseSL = gSpotLight.color.rgb * cos * gSpotLight.intensity * factor * falloffFactor;

	float32_t3 specularSL = gSpotLight.color.rgb * gSpotLight.intensity * specularPow *  factor * falloffFactor;

	output.color.rgb = diffuseSL + specularSL;
	output.color.a = (output.color.r + output.color.g + output.color.b) / 3.0f * 0.6f;

	if (output.color.a == 0) {
		discard;
	}
	
	
	return output;
}