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

	ID3D12DescriptorHeap* GetRTVHeap() { return rtvHeap_.Get(); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetNewRTVCPUDescriptorHandle();

	D3D12_GPU_DESCRIPTOR_HANDLE GetNewRTVGPUDescriptorHandle();

	ID3D12DescriptorHeap* GetDSVHeap() { return dsvHeap_.Get(); }

	D3D12_CPU_DESCRIPTOR_HANDLE GetNewDSVCPUDescriptorHandle();

	D3D12_GPU_DESCRIPTOR_HANDLE GetNewDSVGPUDescriptorHandle();

private:
	DescriptorHeapManager() = default;
	~DescriptorHeapManager() = default;
	DescriptorHeapManager(const DescriptorHeapManager&) = delete;
	DescriptorHeapManager& operator=(const DescriptorHeapManager&) = delete;

	/// <summary>
	/// ディスクリプタヒープの作成関数
	/// </summary>
	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

private:

	ID3D12Device* device_ = nullptr;

	ComPtr<ID3D12DescriptorHeap> srvHeap_;

	ComPtr<ID3D12DescriptorHeap> rtvHeap_;

	ComPtr<ID3D12DescriptorHeap> dsvHeap_;

	/*std::array<DescriptorHandle, DescriptorHeapManager::kNumDescriptors_> handles_;*/

	uint32_t srvCPUDescriptorHandleCount_ = 0;
	uint32_t srvGPUDescriptorHandleCount_ = 0;

	uint32_t rtvCPUDescriptorHandleCount_ = 0;
	uint32_t rtvGPUDescriptorHandleCount_ = 0;

	uint32_t dsvCPUDescriptorHandleCount_ = 0;
	uint32_t dsvGPUDescriptorHandleCount_ = 0;
};