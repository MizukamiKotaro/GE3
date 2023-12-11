#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"

// スプライト
class Sprite
{
public:

	Sprite();
	~Sprite();

	enum class BlendMode
	{
		kBlendModeNone, // ブレンドなし
		kBlendModeNormal, // デフォルト
		kBlendModeAdd, // 加算
		kBlendModeSubtract, // 減算
		kBlendModeMultiply, // 乗算
		kBlendModeScreen, // スクリーン
	};

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

public:

	void LoadTexture(const std::string& filePath);

private:

	ComPtr<ID3D12Resource> vertexResource_;
	VertexData* vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> transformationMatrixResource_;
	TransformationMatrix* transformationMatrixData_;

public:

	Vector2 scale_;
	float rotate_;
	Vector2 pos_;

private:

	Vector2 uvTranslate_;
	Vector2 uvScale_;
	float uvRotate_;

	bool isLoad_;
	uint32_t textureHundle_;

};

