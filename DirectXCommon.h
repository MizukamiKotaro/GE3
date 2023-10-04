#pragma once

#include<d3d12.h>
#include<dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include <vector>

class DirectXCommon
{
public: // メンバ関数

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static DirectXCommon* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WinApp* winApp);

	/// <summary>
	/// デバイスの取得
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDevice() { return device_.Get(); }

	/// <summary>
	/// コマンドリストの取得
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

	/// <summary>
	/// スワップチェーンの取得
	/// </summary>
	/// <returns>スワップチェーン</returns>
	IDXGISwapChain4* GetSwapChain() { return swapChain_.Get(); }

	/// <summary>
	/// スワップチェーンリソースの配列の取得
	/// </summary>
	/// <returns>スワップチェーンの配列</returns>
	std::vector<ComPtr<ID3D12Resource>> GetSwapChainResources() { return swapChainResources_; }

	/// <summary>
	/// コマンドアロケータの取得
	/// </summary>
	/// <returns>コマンドアロケータ</returns>
	ID3D12CommandAllocator* GetCommandAllocator() { return commandAllocator_.Get(); }

	/// <summary>
	/// コマンドキューの取得
	/// </summary>
	/// <returns>コマンドキュー</returns>
	ID3D12CommandQueue* GetCommandQueue() { return commandQueue_.Get(); }

	/// <summary>
	/// dsvディスクリプタヒープの取得
	/// </summary>
	/// <returns>dsvディスクリプタヒープ</returns>
	ID3D12DescriptorHeap* GetDSVDescriptorHeap() { return dsvHeap_.Get(); }

	/// <summary>
	/// rtvハンドルの配列の取得
	/// </summary>
	/// <returns>rtvハンドルの配列</returns>
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> GetRTVHandles() { return rtvHandles_; }

	/// <summary>
	/// フェンスの取得
	/// </summary>
	/// <returns>フェンス</returns>
	ID3D12Fence* GetFence() { return fence_.Get(); }

	/// <summary>
	/// フェンスバリューの取得
	/// </summary>
	/// <returns>フェンスバリュー</returns>
	uint64_t* GetFenceValue() { return &fenceValue_; }

	/// <summary>
	/// フェンスイベントの取得
	/// </summary>
	/// <returns>フェンスイベント</returns>
	HANDLE* GetFenceEvent() { return &fenceEvent_; }


	// 解放処理
	void Finalize();

	/// <summary>
	/// ディスクリプタヒープの作成関数
	/// </summary>
	static ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

private: // メンバ変数

	// ウィンドウズアプリケーション管理
	WinApp* winApp_ = nullptr;

	ComPtr<IDXGIFactory7> dxgiFactory_;
	ComPtr<ID3D12Device> device_;
	IDXGIAdapter4* useAdapter_;
	ComPtr<ID3D12CommandQueue> commandQueue_;
	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;
	ComPtr<IDXGISwapChain4> swapChain_;
	std::vector<ComPtr<ID3D12Resource>> swapChainResources_;
	//ComPtr<ID3D12Resource> swapChainResources_[2] = {}; // ダブルバッファだから
	ComPtr<ID3D12DescriptorHeap> rtvHeap_;
	ComPtr<ID3D12DescriptorHeap> dsvHeap_;
	ComPtr<ID3D12Fence> fence_;
	uint64_t fenceValue_ = 0;
	HANDLE fenceEvent_;

	// 確認用
	//RTVを2つ作るのでハンドルを2つ用意
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandles_;
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2] = {};

private: // メンバ関数

	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

	/// <summary>
	/// DXGIデバイス初期化
	/// </summary>
	void InitializeDXGIDevice();

	/// <summary>
	/// スワップチェーンの生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// コマンド関連初期化
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// レンダーターゲット生成
	/// </summary>
	void CreateFinalRenderTargets();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepthBuffer();

	ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);

	/// <summary>
	/// フェンス生成
	/// </summary>
	void CreateFence();
};