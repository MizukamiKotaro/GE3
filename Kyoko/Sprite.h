#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>

#include "../Math/Vector4.h"

class SpriteCommon;

// スプライト
class Sprite
{
public:

	Sprite() = default;
	~Sprite();

	struct VertexData
	{
		Vector4 position;
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void Draw();

public:

	void SetSpriteCommon(SpriteCommon* spriteCommon) { spriteCommon_ = spriteCommon; }
	void SetCommandList(ID3D12GraphicsCommandList* commandList) { commandList_ = commandList; }

private:


private:

	SpriteCommon* spriteCommon_;
	ID3D12GraphicsCommandList* commandList_;

	ComPtr<ID3D12Resource> vertexResource_;
	VertexData* vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	/*ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> transformationMatrixResource_;
	TransformationMatrix* transformationMatrixData_;*/
};

