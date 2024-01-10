#include "PostEffect.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"
#include "WinApp/WinApp.h"
#include "Externals/DirectXTex/d3dx12.h"

const float PostEffect::clearColor[4] = { 0.25f,0.5f,0.1f,0.0f };

PostEffect::PostEffect(const std::string& filePath, const Vector2& pos, const Vector2& texLeftTop, const Vector2& texSize, const Vector4& color, const Vector2& anchorPoint, bool isFlipX, bool isFlipY)
{

	CreateVertexRes();

	LoadTexture(filePath);
	AdjustTextureSize();

	CreateResources();

	rotate_ = 0.0f;
	pos_ = pos;

	worldMat_ = Matrix4x4::MakeAffinMatrix({ 1.0f,1.0f,0.0f }, { 0.0f,0.0f,rotate_ }, { pos_.x,pos_.y,0.0f });

	anchorPoint_ = anchorPoint;
	textureLeftTop_ = texLeftTop;
	textureSize_ = texSize;

	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;

	TransferSize();
	TransferUV();

	uvTranslate_ = {};
	uvScale_ = { 1.0f,1.0f };
	uvRotate_ = 0.0f;

	SetColor(color);

	Update();
}

PostEffect::PostEffect(uint32_t texHundle, const Vector2& pos, const Vector2& texLeftTop, const Vector2& texSize, const Vector4& color, const Vector2& anchorPoint, bool isFlipX, bool isFlipY)
{
	CreateVertexRes();

	textureHundle_ = texHundle;
	AdjustTextureSize();

	CreateResources();

	rotate_ = 0.0f;
	pos_ = pos;

	worldMat_ = Matrix4x4::MakeAffinMatrix({ 1.0f,1.0f,0.0f }, { 0.0f,0.0f,rotate_ }, { pos_.x,pos_.y,0.0f });

	anchorPoint_ = anchorPoint;
	textureLeftTop_ = texLeftTop;
	textureSize_ = texSize;

	isFlipX_ = isFlipX;
	isFlipY_ = isFlipY;

	TransferSize();
	TransferUV();

	uvTranslate_ = {};
	uvScale_ = { 1.0f,1.0f };
	uvRotate_ = 0.0f;

	SetColor(color);

	Update();
}

PostEffect::~PostEffect()
{
	vertexResource_->Release();
	transformResource_->Release();
	materialResource_->Release();
}

void PostEffect::Initialize()
{
	
}

void PostEffect::Update()
{
	worldMat_ = Matrix4x4::MakeAffinMatrix({ 1.0f,1.0f,0.0f }, { 0.0f,0.0f,rotate_ }, { pos_.x,pos_.y,0.0f });
	TransferSize();
}

void PostEffect::Draw(const Camera& camera, BlendMode blendMode)
{

	if (isInvisible_) {
		return;
	}

	PreDraw();

	transformData_->WVP = worldMat_ * camera.GetOrthographicMat();
	materialData_->uvTransform = Matrix4x4::MakeAffinMatrix({ uvScale_.x,uvScale_.y,0.0f }, { 0.0f,0.0f,uvRotate_ }, { uvTranslate_.x,uvTranslate_.y,0.0f });

	GraphicsPiplineManager::GetInstance()->SetBlendMode(piplineType, static_cast<uint32_t>(blendMode));

	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(2, srvGPUDescriptorHandle_);

	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(6, 1, 0, 0);

}

void PostEffect::PreDrawScene()
{
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	// バリアの変更
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(texResource_.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->ResourceBarrier(1, &barrier);

	// レンダーターゲットのセット
	commandList->OMSetRenderTargets(1, &rtvCPUDescriptorHandle_, false, &dsvCPUDescriptorHandle_);

	// ビューポートの設定
	CD3DX12_VIEWPORT viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::kWindowWidth, WinApp::kWindowHeight);
	commandList->RSSetViewports(1, &viewport);

	// シザリング矩形の設定
	CD3DX12_RECT rect = CD3DX12_RECT(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight);
	commandList->RSSetScissorRects(1, &rect);

	// 全画面クリア
	commandList->ClearRenderTargetView(rtvCPUDescriptorHandle_, clearColor, 0, nullptr);

	// 深度バッファクリア
	commandList->ClearDepthStencilView(dsvCPUDescriptorHandle_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { DescriptorHeapManager::GetInstance()->GetSRVHeap() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps);

	GraphicsPiplineManager::GetInstance()->PreDraw();
}

void PostEffect::PostDrawScene()
{
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	// バリアの変更
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(texResource_.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	commandList->ResourceBarrier(1, &barrier);

}

void PostEffect::LoadTexture(const std::string& filePath)
{

	TextureManager* texManager = TextureManager::GetInstance();

	textureHundle_ = texManager->LoadTexture(filePath);

	isLoad_ = true;
}

void PostEffect::SetTextureHandle(uint32_t textureHundle)
{
	textureHundle_ = textureHundle;

}

void PostEffect::SetAnchorPoint(const Vector2& anchorpoint)
{
	anchorPoint_ = anchorpoint;

	TransferSize();
}

void PostEffect::SetColor(const Vector4& color)
{
	color_.x = std::clamp<float>(color.x, 0.0f, 1.0f);
	color_.y = std::clamp<float>(color.y, 0.0f, 1.0f);
	color_.z = std::clamp<float>(color.z, 0.0f, 1.0f);
	color_.w = std::clamp<float>(color.w, 0.0f, 1.0f);

	materialData_->color = color;
}

void PostEffect::SetIsFlipX(bool isFlipX)
{
	isFlipX_ = isFlipX;

	TransferSize();
}

void PostEffect::SetIsFlipY(bool isFlipY)
{
	isFlipY_ = isFlipY;

	TransferSize();
}

void PostEffect::SetTextureTopLeft(const Vector2& texTopLeft)
{
	textureLeftTop_ = textureLeftTop_;

	TransferUV();
}

void PostEffect::SetTextureSize(const Vector2& texSize)
{
	textureSize_ = texSize;

	TransferUV();
}

void PostEffect::TransferSize()
{
	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;

	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	vertexData_[0].vertexPos = { left,bottom,0.0f,1.0f }; // 左下
	vertexData_[1].vertexPos = { left,top,0.0f,1.0f }; // 左上
	vertexData_[2].vertexPos = { right,bottom,0.0f,1.0f }; // 右下

	vertexData_[3].vertexPos = { left,top,0.0f,1.0f }; // 左上
	vertexData_[4].vertexPos = { right,top,0.0f,1.0f }; // 右上
	vertexData_[5].vertexPos = { right,bottom,0.0f,1.0f }; // 右下
}

void PostEffect::TransferUV()
{
	vertexData_[0].texcoord = { textureLeftTop_.x,textureLeftTop_.y + textureSize_.y }; // 左下
	vertexData_[1].texcoord = textureLeftTop_; // 左上
	vertexData_[2].texcoord = textureLeftTop_ + textureSize_; // 右下

	vertexData_[3].texcoord = textureLeftTop_; // 左上
	vertexData_[4].texcoord = { textureLeftTop_.x + textureSize_.x,textureLeftTop_.y }; // 右上
	vertexData_[5].texcoord = textureLeftTop_ + textureSize_; // 右下
}

void PostEffect::AdjustTextureSize()
{
	D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetTextureDesc(textureHundle_);

	size_ = { static_cast<float>(resDesc.Width),static_cast<float>(resDesc.Height) };
}

void PostEffect::CreateVertexRes()
{
	//Sprite用の頂点リソースを作る
	vertexResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * 6);
	//頂点バッファーを作成する
	//リソースの先頭アドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void PostEffect::CreateMaterialRes()
{
	//マテリアル用のリソースを作る。今回はcolor1つ分を用意する
	materialResource_ = DirectXCommon::CreateBufferResource(sizeof(Material));
	//マテリアルデータを書き込む
	//書き込むためのアドレスを取得\l
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//今回は赤を書き込んでいる
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	//*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , false };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();
}

void PostEffect::CreateTranformRes()
{
	//Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformResource_ = DirectXCommon::CreateBufferResource(sizeof(TransformationMatrix));
	//データを書き込む
	//書き込むためのアドレスを取得
	transformResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformData_));
	//単位行列を書き込んでいく
	transformData_->WVP = { Matrix4x4::MakeIdentity4x4() };
	//*transformationMatrixData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4() };
}

void PostEffect::CreateTexRes()
{

	CD3DX12_RESOURCE_DESC texDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		WinApp::kWindowWidth,
		(UINT)WinApp::kWindowHeight,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	CD3DX12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	CD3DX12_CLEAR_VALUE value = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, clearColor);

	HRESULT hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&properties,
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&value,
		IID_PPV_ARGS(&texResource_)
	);
	assert(SUCCEEDED(hr));

	// イメージデータの転送

	// 画素数
	const UINT pixelCount = WinApp::kWindowWidth * WinApp::kWindowHeight;
	// 画像1行分のデータ
	const UINT rowPitch = sizeof(UINT) * WinApp::kWindowWidth;
	// 画像全体のデータサイズ
	const UINT depthPitch = rowPitch * WinApp::kWindowHeight;

	// 画像イメージ
	UINT* img = new UINT[pixelCount];
	for (int i = 0; i < pixelCount; i++) { img[i] = 0xFF0000FF; }

	// データの転送
	hr = texResource_->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
	assert(SUCCEEDED(hr));
	delete[] img;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	srvCPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewSRVCPUDescriptorHandle();
	srvGPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewSRVGPUDescriptorHandle();

	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(texResource_.Get(), &srvDesc, srvCPUDescriptorHandle_);
}

void PostEffect::CreateRTV()
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //出力結果をSRGBに変換して書き込む
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; //2dテクスチャとして書き込む

	rtvCPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewRTVCPUDescriptorHandle();
	//rtvGPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewRTVGPUDescriptorHandle();

	DirectXCommon::GetInstance()->GetDevice()->CreateRenderTargetView(texResource_.Get(), &rtvDesc, rtvCPUDescriptorHandle_);
}

void PostEffect::CreateDSV()
{
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		WinApp::kWindowWidth,
		(UINT)WinApp::kWindowHeight,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	CD3DX12_HEAP_PROPERTIES properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE value = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D24_UNORM_S8_UINT, 1.0f, 0);

	HRESULT hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&properties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&value,
		IID_PPV_ARGS(&dsvResource_)
	);
	assert(SUCCEEDED(hr));

	dsvCPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewDSVCPUDescriptorHandle();
	//dsvGPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewDSVGPUDescriptorHandle();

	//DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture
	// DSVHeapの先頭にDSVを作る
	DirectXCommon::GetInstance()->GetDevice()->CreateDepthStencilView(dsvResource_.Get(), &dsvDesc, dsvCPUDescriptorHandle_);
}

void PostEffect::CreateResources()
{
	CreateMaterialRes();

	CreateTranformRes();

	CreateTexRes();

	CreateRTV();

	CreateDSV();
}


