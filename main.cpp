#include "Kyoko/WinApp/WinApp.h"
#include "Kyoko/ResourceLeakChecker.h"
#include "Kyoko/DirectXCommon.h"
#include "Input/Input.h"
#include "Kyoko/SpriteCommon.h"
#include "Kyoko/Sprite.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {

	ResourceLeackChecker leakCheck;

#ifdef _DEBUG
	ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		//デバッグレイヤーを有効にする
		debugController->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif // DEBUG

#pragma region 基盤システムの初期化

	WinApp* winApp = WinApp::GetInstance();
	winApp->CreateGameWindow();

	DirectXCommon* dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	Input* input = Input::GetInstance();
	input->Initialize(winApp);

	SpriteCommon* spriteCommon = nullptr;
	spriteCommon = new SpriteCommon();
	spriteCommon->Initialize(dxCommon->GetDevice(), dxCommon->GetCommandList());

	Sprite* sprite = new Sprite();
	sprite->SetSpriteCommon(spriteCommon);
	sprite->SetCommandList(dxCommon->GetCommandList());
	sprite->Initialize();

#pragma endregion 基盤システムの初期化

	ID3D12Device* device_ = dxCommon->GetDevice();

	ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

#ifdef _DEBUG
	ID3D12InfoQueue* infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		//ヤバいエラーの時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		//エラーのときに止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		//抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds[] = {
			//windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーバグ
			//https:://stackoverflow.com/question/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		//解放
		infoQueue->Release();
	}
#endif // _DEBUG

#pragma region 最初のシーンの初期化



#pragma endregion 最初のシーンの初期化
	

	
	//ウィンドウの×ボタンが押されるまでループ
	while (true) {

#pragma region 基盤システムの更新
		//windowにメッセージが来たら最優先で処理させる
		if (winApp->ProcessMessage()) {
			break;
		}

		input->Update();

#pragma endregion 基盤システムの更新


#pragma region 最初のシーンの更新

		sprite->rotate_.y += 0.01f;

		sprite->Update();

#pragma endregion 最初のシーンの更新

		// 描画前処理
		dxCommon->PreDraw();

#pragma region 最初のシーンの描画

		sprite->Draw();


#pragma endregion 最初のシーンの描画

		// 描画後処理
		dxCommon->PostDraw();

	}

#pragma region 最初のシーンの終了



#pragma endregion 最初のシーンの終了

#pragma region 基盤システムの終了

	delete dxCommon;
	winApp->Finalize();

	delete sprite;
	delete spriteCommon;

#ifdef _DEBUG
	debugController->Release();
#endif // _DEBUG

#pragma endregion 基盤システムの終了

	return 0;
}