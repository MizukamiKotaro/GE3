#include "DescriptorHeap.h"
#include <cassert>
#include "Engine/Base/DebugLog/DebugLog.h"
#include <format>
#include "Externals/DirectXTex/d3dx12.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"

DescriptorHeap::~DescriptorHeap()
{
	descriptors_.clear();
	heap_->Release();
}

void DescriptorHeap::DeleteDescriptor(const DescriptorHandles* handles) 
{
	if (descriptors_.size() != 0) {
		for (std::unique_ptr<Descriptor>& descriptor : descriptors_) {
			if (descriptor->handles.no == handles->no) {
				descriptor->isUse = false;
			}
		}
	}
}

const DescriptorHandles* DescriptorHeap::GetNewDescriptorHandles()
{
	if (descriptors_.size() != 0) {
		for (std::unique_ptr<Descriptor>& descriptor : descriptors_) {
			if (!descriptor->isUse) {
				descriptor->isUse = true;
				return &descriptor->handles;
			}
		}
	}

	Descriptor descriptor;

	ID3D12Device* device = DirectXCommon::GetInstance()->GetDevice();

	descriptor.isUse = true;
	descriptor.handles.no = descriptors_.size();
	descriptor.handles.cpuHandle = heap_->GetCPUDescriptorHandleForHeapStart();
	descriptor.handles.cpuHandle.ptr += (device->GetDescriptorHandleIncrementSize(heapType_) * descriptor.handles.no);

	if (heap_->GetDesc().Flags == D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) {
		descriptor.handles.gpuHandle = heap_->GetGPUDescriptorHandleForHeapStart();
		descriptor.handles.gpuHandle.ptr += (device->GetDescriptorHandleIncrementSize(heapType_) * descriptor.handles.no);
	}
	
	descriptors_.push_back(std::make_unique<Descriptor>(descriptor));
	
	return &descriptors_.back()->handles;
}

ID3D12DescriptorHeap* DescriptorHeap::CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	//ディスクリプタヒープの生成
	ID3D12DescriptorHeap* descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	//ディスクリプタヒープが作られなかったので起動しない
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}
