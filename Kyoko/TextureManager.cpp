#include "TextureManager.h"
#include <cassert>
#include "DebugLog.h"
#include<format>

void TextureManager::Initialize(ID3D12Device* device)
{
	assert(device);
	device_ = device;

	CreateSRVHeap();
}

void TextureManager::Finalize()
{
	for (uint32_t texNum = 0; texNum < static_cast<uint32_t>(textures_.size()); texNum++) {
		textures_[texNum].resource_->Release();
	}
	srvHeap_->Release();
}

uint32_t TextureManager::LoadTexture(const std::string& filePath)
{
	
	for (uint32_t texNum = 0; texNum < static_cast<uint32_t>(textures_.size()); texNum++) {

		if (textures_[texNum].filePath_ == filePath) {
			return texNum;
		}
	}

	DirectX::ScratchImage mipImages = Load(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	ID3D12Resource* textureResource = CreateTextureResource(metadata);

	//metadataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVCPUHandle = GetCPUDescriptorHandle(static_cast<uint32_t>(textures_.size()) + 1);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSRVGPUHandle = GetGPUDescriptorHandle(static_cast<uint32_t>(textures_.size()) + 1);

	device_->CreateShaderResourceView(textureResource, &srvDesc, textureSRVCPUHandle);

	textures_.push_back(Texture(textureResource, textureSRVCPUHandle, textureSRVGPUHandle, filePath));

	return static_cast<uint32_t>(textures_.size()) - 1;
}

DirectX::ScratchImage TextureManager::Load(const std::string& filePath)
{
	//デスクトップファイルを読んでプログラムで使えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = DebugLog::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	//ミップマップの生成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImages;
}

ID3D12Resource* TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata)
{
	//1.metadateを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width); // Textureの幅
	resourceDesc.Height = UINT(metadata.height); // Textureの高さ
	resourceDesc.MipLevels = UINT(metadata.mipLevels); // mipmapの数
	resourceDesc.DepthOrArraySize = UINT(metadata.arraySize); // 奥行き　or 配列Textureの配列数
	resourceDesc.Format = metadata.format; // TextureのFormat
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	//2.利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	//heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	//heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0; // プロセッサの近くに配置

	//3.Resourceの生成
	ID3D12Resource* resource = nullptr;
	HRESULT hr = device_->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定。特になし
		&resourceDesc, // Resorceの設定
		D3D12_RESOURCE_STATE_COPY_DEST, // データ転送される設定
		nullptr, // Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource)); // 作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));
	return resource;
}

void TextureManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {

		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);

		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(hr));
	}
}

void TextureManager::CreateSRVHeap()
{
	
	//ディスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorHeapDesc.NumDescriptors = kNumDescriptors_;
	descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT hr = device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(srvHeap_.GetAddressOf()));
	//ディスクリプタヒープが作られなかったので起動しない
	assert(SUCCEEDED(hr));
	
}

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = srvHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = srvHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index);
	return handleGPU;
}
