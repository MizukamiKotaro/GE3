#include "DescriptorHeapManager.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"

SRVDescriptorHeap::SRVDescriptorHeap()
{
	heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptors_.clear();
	heap_ = DescriptorHeap::CreateDescriptorHeap(DirectXCommon::GetInstance()->GetDevice(), heapType_, kNumDescriptors_, true);
}

RTVDescriptorHeap::RTVDescriptorHeap()
{
	heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptors_.clear();
	heap_ = DescriptorHeap::CreateDescriptorHeap(DirectXCommon::GetInstance()->GetDevice(), heapType_, kNumDescriptors_, false);
}

DSVDescriptorHeap::DSVDescriptorHeap()
{
	heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	descriptors_.clear();
	heap_ = DescriptorHeap::CreateDescriptorHeap(DirectXCommon::GetInstance()->GetDevice(), heapType_, kNumDescriptors_, false);
}

DescriptorHeapManager* DescriptorHeapManager::GetInstance()
{
	static DescriptorHeapManager instance;
	return &instance;
}

void DescriptorHeapManager::Initialize()
{
	srvHeap_ = std::make_unique<SRVDescriptorHeap>();
	rtvHeap_ = std::make_unique<RTVDescriptorHeap>();
	dsvHeap_ = std::make_unique<DSVDescriptorHeap>();
}

void DescriptorHeapManager::Finalize()
{
	srvHeap_.reset();
	rtvHeap_.reset();
	dsvHeap_.reset();
}
