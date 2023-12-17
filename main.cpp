#include "Engine/Base/WinApp/WinApp.h"
#include "Engine/Base/ResourceLeakChecker.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Input/Input.h"
#include "SpriteCommon/SpriteCommon.h"
#include "SpriteCommon/Sprite/Sprite.h"
#include "TextureManager/TextureManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include "externals/imgui/imgui.h"
#include "ModelCommon/ModelCommon.h"
#include "GlobalVariables/GlobalVariables.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "ModelCommon/Model/Model.h"
#include "Utils/Camera/Camera.h"
#include "ModelCommon/ModelData/ModelDataManager/ModelDataManager.h"
#include "ParticleCommon/ParticleCommon.h"
#include "ParticleCommon/Particle/Particle.h"
#include "Utils/RandomGenerator/RandomGenerator.h"

static ResourceLeackChecker leakCheck;

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {

	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	SUCCEEDED(hr);

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

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize();

	DescriptorHeapManager* descriptorHeapManager = DescriptorHeapManager::GetInstance();
	descriptorHeapManager->Initialize();

	TextureManager* textureManager = TextureManager::GetInstance();
	textureManager->Initialize();

	ModelCommon* modelCommon = ModelCommon::GetInstance();
	modelCommon->Initialize();

	ParticleCommon* particleCommon = ParticleCommon::GetInstance();
	particleCommon->Initialize();

	ModelDataManager* modelManager = ModelDataManager::GetInstance();

	Input* input = Input::GetInstance();
	input->Initialize();

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	globalVariables->LoadFiles();

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

	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>("Resources/uvChecker.png");
	sprite->pos_.x += 100.0f;
	sprite->Update();
	std::unique_ptr<Sprite> sprite1 = std::make_unique<Sprite>("Resources/uvChecker.png");
	sprite1->Update();

	uint32_t mesh1 = modelManager->LoadObj("Cube");
	uint32_t mesh2 = modelManager->LoadObj("weapon");

	bool isMesh1 = true;

	std::unique_ptr<Model> model = std::make_unique<Model>(mesh1);
	model->Initialize();

	std::unique_ptr<Particle> particle = std::make_unique<Particle>("plane");
	particle->Initialize();

	Camera camera;
	camera.Initialize();
	camera.transform_.translate_ = { 0.0f,2.0f,-50.0f };

#pragma endregion 最初のシーンの初期化
	
	ImGuiManager::Initialize();
	
	//ウィンドウの×ボタンが押されるまでループ
	while (true) {

#pragma region 基盤システムの更新
		//windowにメッセージが来たら最優先で処理させる
		if (winApp->ProcessMessage()) {
			break;
		}
		ImGuiManager::Begin();
		input->Update();

		globalVariables->Update();

#pragma endregion 基盤システムの更新


#pragma region 最初のシーンの更新


		if (input->PressedKey(DIK_SPACE)) {
			if (isMesh1) {
				isMesh1 = false;
				model->SetMesh(mesh2);
			}
			else {
				isMesh1 = true;
				model->SetMesh(mesh1);
			}
		}

		if (input->PressedKey(DIK_A)) {
			model->SetTex(textureManager->LoadTexture("Resources/uvChecker.png"));
		}

		ImGui::Begin("a");
		ImGui::SliderFloat3("a", &model->transform_.translate_.x, -100.0f, 100.0f);
		ImGui::End();

		model->Update();
		particle->Update();
		camera.Update();
		

#pragma endregion 最初のシーンの更新

		ImGuiManager::End();

		// 描画前処理
		dxCommon->PreDraw();

#pragma region 最初のシーンの描画

		spriteCommon->PreDraw();
		sprite->Draw(camera.GetOrthographicMat(), Sprite::BlendMode::kBlendModeNormal);
		sprite1->Draw(camera.GetOrthographicMat(), Sprite::BlendMode::kBlendModeAdd);

		modelCommon->PreDraw();
		model->Draw(camera.GetViewProjection());

		ParticleCommon::GetInstance()->PreDraw();
		particle->Draw(camera.GetViewProjection());

		spriteCommon->PreDraw();

#pragma endregion 最初のシーンの描画

		ImGuiManager::Draw();

		// 描画後処理
		dxCommon->PostDraw();

	}

#pragma region 最初のシーンの終了



#pragma endregion 最初のシーンの終了

#pragma region 基盤システムの終了
	ImGuiManager::Finalize();

	CoUninitialize();
	textureManager->Finalize();
	modelManager->Finalize();
	descriptorHeapManager->Finalize();
	dxCommon->Finalize();
	winApp->Finalize();

#ifdef _DEBUG
	debugController->Release();
#endif // _DEBUG

#pragma endregion 基盤システムの終了

	return 0;
}

