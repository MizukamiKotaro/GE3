#include "Sprite.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
	float32_t4x4 uvTransform;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct HighLumiData {
	float min;
	float max;
	int32_t isToWhite;
};
ConstantBuffer<HighLumiData> gHighLumi : register(b1);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;

	float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

	float grayScale = textureColor.r * 0.299f + textureColor.g * 0.587f + textureColor.b * 0.114f;
	float extract = smoothstep(gHighLumi.min, gHighLumi.max, grayScale);

	output.color = textureColor * extract;

	if (gHighLumi.isToWhite == 1) {
		if (output.color.r != 0.0f && output.color.g != 0.0f && output.color.b != 0.0f && output.color.a != 0.0f) {
			output.color.r = 1.0f;
			output.color.g = 1.0f;
			output.color.b = 1.0f;
			output.color.a = 1.0f;
		}
	}

	return output;
}