#include "Contrast.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
	//int32_t enableLighting;
	float32_t4x4 uvTransform;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

struct ContrastData {
	float brightness;
	float contrast;
};
ConstantBuffer<ContrastData> gContrast : register(b1);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

	output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
	output.color.a = gMaterial.color.a * textureColor.a;

	if (output.color.a == 0) {
		discard;
	}

	output.color.r = clamp(pow(output.color.r + gContrast.brightness, gContrast.contrast), 0.0f, 1.0f);
	output.color.g = clamp(pow(output.color.g + gContrast.brightness, gContrast.contrast), 0.0f, 1.0f);
	output.color.b = clamp(pow(output.color.b + gContrast.brightness, gContrast.contrast), 0.0f, 1.0f);

	
	return output;
}