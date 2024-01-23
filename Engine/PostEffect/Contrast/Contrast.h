#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"
#include "GraphicsPipelines/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"

// スプライト
class Contrast
{
public:

	Contrast();

	~Contrast();

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

	struct ContrastData {
		float brightness_;
		float contrast_;
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

	const D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle() const { return srvHandles_->gpuHandle; };

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

	void CreateContrastRes();

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

	ComPtr<ID3D12Resource> contrastResource_;
	ContrastData* contrastData_;

	ComPtr<ID3D12Resource> transformResource_;
	TransformationMatrix* transformData_;

	ComPtr<ID3D12Resource> texResource_;
	const DescriptorHandles* srvHandles_;

	ComPtr<ID3D12Resource> rtvResource_;
	const DescriptorHandles* rtvHandles_;

	ComPtr<ID3D12Resource> dsvResource_;
	const DescriptorHandles* dsvHandles_;

private:

	static const GraphicsPiplineManager::PiplineType piplineType = GraphicsPiplineManager::PiplineType::CONTRAST;

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

