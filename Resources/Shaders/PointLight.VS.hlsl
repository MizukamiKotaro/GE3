#include "PointLight.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
	float32_t4x4 World;
	float32_t4x4 WorldInverse;
};
ConstantBuffer<TransformationMatrix>gTransformationMatrix : register(b0);

struct VertexShaderInput {
	float32_t4 vertexPos : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.vertexPos = mul(input.vertexPos, gTransformationMatrix.WVP);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrix.WorldInverse));
	output.worldPosition = mul(input.vertexPos, gTransformationMatrix.World).xyz;
	return output;
}