#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <string>
#include "Externals/DirectXTex/DirectXTex.h"
#include <list>
#include <memory>

static const size_t kNumDescriptors_ = 256;

struct DescriptorHandles
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	size_t no;
};

struct Descriptor
{
	DescriptorHandles handles;
	bool isUse;
};

class DescriptorHeap {
public:
	
	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	~DescriptorHeap();

public:

	void DeleteDescriptor(const DescriptorHandles* handles);

	ID3D12DescriptorHeap* GetHeap() { return heap_.Get(); }

	const DescriptorHandles* GetNewDescriptorHandles();

	static ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);


protected:

	ComPtr<ID3D12DescriptorHeap> heap_;

	D3D12_DESCRIPTOR_HEAP_TYPE heapType_;

	std::list<std::unique_ptr<Descriptor>> descriptors_;
};