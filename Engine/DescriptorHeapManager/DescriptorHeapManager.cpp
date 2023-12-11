#include "DescriptorHeapManager.h"
#include <cassert>
#include "Engine/DebugLog/DebugLog.h"
#include <format>
#include "Externals/DirectXTex/d3dx12.h"
#include "Engine/DirectXCommon/DirectXCommon.h"

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

	CreateSRVHeap();
}

void DescriptorHeapManager::Finalize()
{
	srvHeap_->Release();
}


void DescriptorHeapManager::CreateSRVHeap()
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
