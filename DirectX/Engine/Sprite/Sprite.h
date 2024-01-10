#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"
#include "GraphicsPipelines/GraphicsPiplineManager/GraphicsPiplineManager.h"

class Camera;

// スプライト
class Sprite
{
public:

	Sprite(const std::string& filePath, const Vector2& pos = { 0.0f,0.0f }, const Vector2& texLeftTop = {}, const Vector2& texSize = {1.0f,1.0f},
		const Vector4 & color = { 1.0f,1.0f,1.0f,1.0f }, const Vector2& anchorPoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);
	Sprite(uint32_t texHundle, const Vector2& pos = { 0.0f,0.0f }, const Vector2& texLeftTop = {}, const Vector2& texSize = {1.0f,1.0f},
		const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, const Vector2& anchorPoint = { 0.5f,0.5f }, bool isFlipX = false, bool isFlipY = false);
	~Sprite();

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

	void Draw(const Camera& camera,  BlendMode blendMode = BlendMode::kBlendModeNormal);

	static void PreDraw() { GraphicsPiplineManager::GetInstance()->PreDraw(piplineType); }

public:

	void LoadTexture(const std::string& filePath);

	void SetTextureHandle(uint32_t textureHundle);

	void SetAnchorPoint(const Vector2& anchorpoint);

	void SetColor(const Vector4& color);

	void SetIsFlipX(bool isFlipX);

	void SetIsFlipY(bool isFlipY);

	void SetTextureTopLeft(const Vector2& texTopLeft);

	void SetTextureSize(const Vector2& texSize);

private:
	Sprite() = default;

	void TransferSize();

	void TransferUV();

	void AdjustTextureSize();

	void CreateVertexRes();

	void CreateMaterialRes();

	void CreateTranformRes();

private:

	ComPtr<ID3D12Resource> vertexResource_;
	VertexData* vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> transformResource_;
	TransformationMatrix* transformData_;

public:

	float rotate_;
	Vector2 pos_;
	Vector2 size_;

private:

	static const GraphicsPiplineManager::PiplineType piplineType = GraphicsPiplineManager::PiplineType::SPRITE;

	Matrix4x4 worldMat_;

	Vector2 uvTranslate_;
	Vector2 uvScale_;
	float uvRotate_;

	Vector4 color_;

	Vector2 anchorPoint_;

	Vector2 textureLeftTop_;

	Vector2 textureSize_;

	bool isFlipX_ = false;
	bool isFlipY_ = false;

	bool isInvisible_ = false;

	bool isLoad_;
	uint32_t textureHundle_;

};

