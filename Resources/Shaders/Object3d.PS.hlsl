#include "Object3d.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
	float32_t shininess;
	float32_t3 speqularColor;
};
ConstantBuffer<Material> gMaterial : register(b0);
struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

struct DirectionalLight {
	float32_t4 color;
	float32_t3 direction;
	float intensity;
};
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct PointLight {
	float32_t4 color;
	float32_t3 position;
	float intensity;
	float radius;
	float decay;
};
ConstantBuffer<PointLight> gPointLight : register(b3);

struct Camera {
	float32_t3 worldPosition;
};
ConstantBuffer<Camera> gCamera : register(b2);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

	if (gMaterial.enableLighting == 1) {
		float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
		output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		output.color.a = gMaterial.color.a * textureColor.a;
	}
	else if (gMaterial.enableLighting == 2) {
		float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
		output.color.a = gMaterial.color.a * textureColor.a;
	}
	else if (gMaterial.enableLighting == 3) {
		float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

		float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
		/*float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
		float RdotE = dot(reflectLight, toEye);
		float specularPow = pow(saturate(RdotE), gMaterial.shininess);*/
		float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
		float NdotH = dot(normalize(input.normal), halfVector);
		float specularPow = pow(saturate(NdotH), gMaterial.shininess);

		float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;

		float32_t3 specular = gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * gMaterial.speqularColor;

		// pointLight
		float32_t3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
		NdotL = dot(normalize(input.normal), -pointLightDirection);
		cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
		halfVector = normalize(-pointLightDirection + toEye);
		NdotH = dot(normalize(input.normal), halfVector);
		specularPow = pow(saturate(NdotH), gMaterial.shininess);

		float32_t distance = length(gPointLight.position - input.worldPosition);
		float32_t factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);

		float32_t3 diffusePL = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;

		float32_t3 specularPL = gPointLight.color.rgb * gPointLight.intensity * specularPow * gMaterial.speqularColor * factor;

		output.color.rgb = diffuse + specular + diffusePL + specularPL;
		output.color.a = gMaterial.color.a * textureColor.a;
	}
	else {
		output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
		output.color.a = gMaterial.color.a * textureColor.a;
	}

	if (textureColor.a <= 0.5 || output.color.a == 0) {
		discard;
	}
	
	
	return output;
}