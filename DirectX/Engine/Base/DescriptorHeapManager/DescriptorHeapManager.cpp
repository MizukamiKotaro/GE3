#include "DescriptorHeapManager.h"
#include <cassert>
#include "Engine/Base/DebugLog/DebugLog.h"
#include <format>
#include "Externals/DirectXTex/d3dx12.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"

DescriptorHeapManager* DescriptorHeapManager::GetInstance()
{
	static DescriptorHeapManager instance;
	return &instance;
}

void DescriptorHeapManager::Initialize()
{
	device_ = DirectXCommon::GetInstance()->GetDevice();
	// ImGui用
	srvCPUDescriptorHandleCount_ = 1;
	srvGPUDescriptorHandleCount_ = 1;

	rtvCPUDescriptorHandleCount_ = 2;
	rtvGPUDescriptorHandleCount_ = 2;

	dsvCPUDescriptorHandleCount_ = 1;
	dsvGPUDescriptorHandleCount_ = 1;

	rtvHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kNumDescriptors_, false);
	dsvHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kNumDescriptors_, false);
	srvHeap_ = CreateDescriptorHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kNumDescriptors_, true);
}

void DescriptorHeapManager::Finalize()
{
	rtvHeap_->Release();
	dsvHeap_->Release();
	srvHeap_->Release();
}

ID3D12DescriptorHeap* DescriptorHeapManager::CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
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

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetNewSRVCPUDescriptorHandle() {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = srvHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * srvCPUDescriptorHandleCount_);
	srvCPUDescriptorHandleCount_++;
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetNewSRVGPUDescriptorHandle() {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = srvHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * srvGPUDescriptorHandleCount_);
	srvGPUDescriptorHandleCount_++;
	return handleGPU;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetNewRTVCPUDescriptorHandle()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * rtvCPUDescriptorHandleCount_);
	rtvCPUDescriptorHandleCount_++;
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetNewRTVGPUDescriptorHandle()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = rtvHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * rtvGPUDescriptorHandleCount_);
	rtvGPUDescriptorHandleCount_++;
	return handleGPU;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetNewDSVCPUDescriptorHandle()
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = dsvHeap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV) * dsvCPUDescriptorHandleCount_);
	dsvCPUDescriptorHandleCount_++;
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetNewDSVGPUDescriptorHandle()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = dsvHeap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV) * dsvGPUDescriptorHandleCount_);
	dsvGPUDescriptorHandleCount_++;
	return handleGPU;
}
