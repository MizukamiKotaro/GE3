//#include "Sprite.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
	//float32_t4x4 World;
};
ConstantBuffer<TransformationMatrix>gTransformationMatrix : register(b0);

struct VertexShaderOutput {
	float32_t4 vertexPos : SV_POSITION;
};

struct VertexShaderInput {
	float32_t4 vertexPos : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.vertexPos = mul(input.vertexPos, gTransformationMatrix.WVP);
	return output;
}