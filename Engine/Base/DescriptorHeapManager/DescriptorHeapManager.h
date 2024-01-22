#pragma once

#include "DescriptorHeap/DescriptorHeap.h"
#include <memory>

class SRVDescriptorHeap : public DescriptorHeap
{
public:
	SRVDescriptorHeap();
};

class RTVDescriptorHeap : public DescriptorHeap
{
public:
	RTVDescriptorHeap();
};

class DSVDescriptorHeap : public DescriptorHeap
{
public:
	DSVDescriptorHeap();
};

class DescriptorHeapManager {
public:

	static DescriptorHeapManager* GetInstance();

	void Initialize();

	void Finalize();

	DescriptorHeap* GetSRVDescriptorHeap() { return srvHeap_.get(); }

	DescriptorHeap* GetRTVDescriptorHeap() { return rtvHeap_.get(); }

	DescriptorHeap* GetDSVDescriptorHeap() { return dsvHeap_.get(); }

private:
	DescriptorHeapManager() = default;
	~DescriptorHeapManager() = default;
	DescriptorHeapManager(const DescriptorHeapManager&) = delete;
	DescriptorHeapManager& operator=(const DescriptorHeapManager&) = delete;

private:

	std::unique_ptr<DescriptorHeap> srvHeap_;

	std::unique_ptr<DescriptorHeap> rtvHeap_;

	std::unique_ptr<DescriptorHeap> dsvHeap_;
};