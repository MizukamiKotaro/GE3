#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "../Math/Vector4.h"
#include "../Math/Matrix4x4.h"

class SpriteCommon;

// スプライト
class Sprite
{
public:

	Sprite() = default;
	~Sprite();

	struct VertexData
	{
		Vector4 vertexPos;
	};

	struct Material
	{
		Vector4 color;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		//Matrix4x4 World;
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void Update();

	void Draw();

public:

	void LoadTexture(const std::string& filePath);

	void SetSpriteCommon(SpriteCommon* spriteCommon) { spriteCommon_ = spriteCommon; }
	void SetCommandList(ID3D12GraphicsCommandList* commandList) { commandList_ = commandList; }

private:


private:

	SpriteCommon* spriteCommon_;
	ID3D12GraphicsCommandList* commandList_;

	ComPtr<ID3D12Resource> vertexResource_;
	VertexData* vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> transformationMatrixResource_;
	TransformationMatrix* transformationMatrixData_;

public:

	Vector3 scale_;
	Vector3 rotate_;
	Vector3 pos_;

private:

	bool isLoad_;
	uint16_t textureHundle_;

};

