#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <string>
#include "../externals/DirectXTex/DirectXTex.h"
#include <vector>

class TextureManager {
public:

	static const size_t kNumDescriptors_ = 256;
	
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	TextureManager() = default;
	~TextureManager() = default;

	void Initialize(ID3D12Device* device);

	void Finalize();

	struct Texture {
		ComPtr<ID3D12Resource> resource_;
		D3D12_CPU_DESCRIPTOR_HANDLE srvCPUDescriptorHandle_;
		D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle_;
		std::string filePath_;
	};

public:

	const D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t textureHandle);

	uint32_t LoadTexture(const std::string& filePath);

private:
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	DirectX::ScratchImage Load(const std::string& filePath);

	ID3D12Resource* CreateTextureResource(const DirectX::TexMetadata& metadata);

	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	void CreateSRVHeap();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);

private:

	ID3D12Device* device_;

	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	std::vector<Texture> textures_;

};