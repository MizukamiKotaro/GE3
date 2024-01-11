#pragma once

#include <vector>
#include <wrl.h>
#include "VertexData3D.h"
#include <d3d12.h>

struct MeshData
{
	std::vector<VertexData> verteces;
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_;
	VertexData* vertexData_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
};