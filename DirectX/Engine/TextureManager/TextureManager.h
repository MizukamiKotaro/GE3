#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <string>
#include "Externals/DirectXTex/DirectXTex.h"
#include <vector>

class TextureManager {
public:
	
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static TextureManager* GetInstance();

	void Initialize();

	void Finalize();

	struct Texture {
		ComPtr<ID3D12Resource> resource_;
		ComPtr<ID3D12Resource> intermediateResource_;
		D3D12_CPU_DESCRIPTOR_HANDLE srvCPUDescriptorHandle_;
		D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle_;
		std::string filePath_;
	};

public:

	const D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(const uint32_t& handle) const
	{
		return textures_[handle].srvGPUDescriptorHandle_;
	};

	const D3D12_RESOURCE_DESC GetTextureDesc(uint32_t handle) const { return textures_[handle].resource_->GetDesc(); }

	uint32_t LoadTexture(const std::string& filePath);

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	DirectX::ScratchImage Load(const std::string& filePath);

	ID3D12Resource* CreateTextureResource(const DirectX::TexMetadata& metadata);

	//void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

private:

	ID3D12Device* device_ = nullptr;

	std::vector<Texture> textures_;

};