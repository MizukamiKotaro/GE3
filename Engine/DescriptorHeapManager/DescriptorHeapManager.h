#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <string>
#include "Externals/DirectXTex/DirectXTex.h"
#include <vector>
#include <array>

class DescriptorHeapManager {
public:
	
	static const size_t kNumDescriptors_ = 256;

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static DescriptorHeapManager* GetInstance();

	void Initialize();

	void Finalize();

	/*struct DescriptorHandle {
		D3D12_CPU_DESCRIPTOR_HANDLE srvCPUDescriptorHandle_;
		D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle_;
	};*/

public:

	/*const D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(const uint32_t& handle) const 
	{ return handles_[handle].srvGPUDescriptorHandle_; };*/

	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.Get(); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetNewSRVCPUDescriptorHandle();

	D3D12_GPU_DESCRIPTOR_HANDLE GetNewSRVGPUDescriptorHandle();

private:
	DescriptorHeapManager() = default;
	~DescriptorHeapManager() = default;
	DescriptorHeapManager(const DescriptorHeapManager&) = delete;
	DescriptorHeapManager& operator=(const DescriptorHeapManager&) = delete;

	void CreateSRVHeap();

private:

	ID3D12Device* device_;

	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	/*std::array<DescriptorHandle, DescriptorHeapManager::kNumDescriptors_> handles_;*/

	uint32_t srvCPUDescriptorHandleCount_;
	uint32_t srvGPUDescriptorHandleCount_;
};