#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"
#include "GraphicsPipelines/GraphicsPiplineManager/GraphicsPiplineManager.h"

// スプライト
class HighLumi
{
public:

	HighLumi();

	~HighLumi();

	struct VertexData
	{
		Vector4 vertexPos;
		Vector2 texcoord;
	};

	struct Material
	{
		Vector4 color;
		Matrix4x4 uvTransform;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		//Matrix4x4 World;
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void Update();

	void Draw(BlendMode blendMode = BlendMode::kBlendModeNormal);


	void PreDrawScene();

	void PostDrawScene();

private:
	static void PreDraw() { GraphicsPiplineManager::GetInstance()->PreDraw(piplineType); }

public:

	const D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle() const { return srvGPUDescriptorHandle_; };

private:

	void SetAnchorPoint(const Vector2& anchorpoint);

	void SetColor(const Vector4& color);

	void SetTextureTopLeft(const Vector2& texTopLeft);

	void SetTextureSize(const Vector2& texSize);

	void TransferSize();

	void TransferUV();

	void CreateVertexRes();

	void CreateMaterialRes();

	void CreateTranformRes();

	void CreateTexRes();

	void CreateRTV();

	void CreateDSV();

	void CreateResources();

private:

	ComPtr<ID3D12Resource> vertexResource_;
	VertexData* vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> transformResource_;
	TransformationMatrix* transformData_;

	ComPtr<ID3D12Resource> texResource_;
	D3D12_CPU_DESCRIPTOR_HANDLE srvCPUDescriptorHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle_;

	ComPtr<ID3D12Resource> rtvResource_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvCPUDescriptorHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE rtvGPUDescriptorHandle_;

	ComPtr<ID3D12Resource> dsvResource_;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUDescriptorHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE dsvGPUDescriptorHandle_;

private:

	static const GraphicsPiplineManager::PiplineType piplineType = GraphicsPiplineManager::PiplineType::HIGH_LUMI;

	static const float clearColor[4];

	Matrix4x4 worldMat_;

	float rotate_;
	Vector2 pos_;
	Vector2 size_;

	Vector2 uvTranslate_;
	Vector2 uvScale_;
	float uvRotate_;

	Vector4 color_;

	Vector2 anchorPoint_;

	Vector2 textureLeftTop_;

	Vector2 textureSize_;

	bool isInvisible_ = false;

};

