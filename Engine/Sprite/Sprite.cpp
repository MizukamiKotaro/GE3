#include "Sprite.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Camera.h"

Sprite::Sprite(const std::string& filePath, const Vector2& pos, const Vector2& texLeftTop, const Vector2& texSize, const Vector4& color, const Vector2& anchorPoint, bool isFlipX, bool isFlipY)
{

	CreateVertexRes();

	LoadTexture(filePath);
	AdjustTextureSize();

	CreateMaterialRes();
	
	CreateTranformRes();

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

Sprite::Sprite(uint32_t texHundle, const Vector2& pos, const Vector2& texLeftTop, const Vector2& texSize, const Vector4& color, const Vector2& anchorPoint, bool isFlipX, bool isFlipY)
{
	CreateVertexRes();

	textureHundle_ = texHundle;
	AdjustTextureSize();

	CreateMaterialRes();

	CreateTranformRes();

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

Sprite::~Sprite()
{
	vertexResource_->Release();
	transformResource_->Release();
	materialResource_->Release();
}

void Sprite::Initialize()
{
	
}

void Sprite::Update()
{
	worldMat_ = Matrix4x4::MakeAffinMatrix({ 1.0f,1.0f,0.0f }, { 0.0f,0.0f,rotate_ }, { pos_.x,pos_.y,0.0f });
	TransferSize();
}

void Sprite::Draw(const Camera& camera, BlendMode blendMode)
{

	if (isInvisible_) {
		return;
	}

	PreDraw();

	transformData_->WVP = worldMat_ * camera.GetOrthographicMat();
	materialData_->uvTransform = Matrix4x4::MakeAffinMatrix({ uvScale_.x,uvScale_.y,0.0f }, { 0.0f,0.0f,uvRotate_ }, { uvTranslate_.x,uvTranslate_.y,0.0f });

	TextureManager* texManager = TextureManager::GetInstance();

	GraphicsPiplineManager::GetInstance()->SetBlendMode(piplineType, static_cast<uint32_t>(blendMode));

	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformResource_->GetGPUVirtualAddress());
	if (isLoad_) {
		commandList->SetGraphicsRootDescriptorTable(2, texManager->GetSRVGPUDescriptorHandle(textureHundle_));
	}
	else {
		this->LoadTexture("Resources/white.png");
		commandList->SetGraphicsRootDescriptorTable(2, texManager->GetSRVGPUDescriptorHandle(textureHundle_));
	}
	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(6, 1, 0, 0);

}

void Sprite::LoadTexture(const std::string& filePath)
{

	TextureManager* texManager = TextureManager::GetInstance();

	textureHundle_ = texManager->LoadTexture(filePath);

	isLoad_ = true;
}

void Sprite::SetTextureHandle(uint32_t textureHundle)
{
	textureHundle_ = textureHundle;

}

void Sprite::SetAnchorPoint(const Vector2& anchorpoint)
{
	anchorPoint_ = anchorpoint;

	TransferSize();
}

void Sprite::SetColor(const Vector4& color)
{
	color_.x = std::clamp<float>(color.x, 0.0f, 1.0f);
	color_.y = std::clamp<float>(color.y, 0.0f, 1.0f);
	color_.z = std::clamp<float>(color.z, 0.0f, 1.0f);
	color_.w = std::clamp<float>(color.w, 0.0f, 1.0f);

	materialData_->color = color;
}

void Sprite::SetIsFlipX(bool isFlipX)
{
	isFlipX_ = isFlipX;

	TransferSize();
}

void Sprite::SetIsFlipY(bool isFlipY)
{
	isFlipY_ = isFlipY;

	TransferSize();
}

void Sprite::SetTextureTopLeft(const Vector2& texTopLeft)
{
	if (texTopLeft.x < 1.0f && texTopLeft.y < 1.0f) {
		textureLeftTop_ = textureLeftTop_;
	}
	else {
		D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetTextureDesc(textureHundle_);

		Vector2 size = { static_cast<float>(resDesc.Width),static_cast<float>(resDesc.Height) };

		textureLeftTop_ = { texTopLeft.x / size.x,texTopLeft.y / size.y };
	}

	TransferUV();
}

void Sprite::SetTextureSize(const Vector2& texSize)
{
	if (texSize.x <= 1.0f && texSize.y <= 1.0f) {
		textureSize_ = texSize;
	}
	else {
		D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetTextureDesc(textureHundle_);

		Vector2 size = { static_cast<float>(resDesc.Width),static_cast<float>(resDesc.Height) };

		textureSize_ = { texSize.x / size.x,texSize.y / size.y };
	}

	TransferUV();
}

void Sprite::TransferSize()
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

void Sprite::TransferUV()
{
	vertexData_[0].texcoord = { textureLeftTop_.x,textureLeftTop_.y + textureSize_.y }; // 左下
	vertexData_[1].texcoord = textureLeftTop_; // 左上
	vertexData_[2].texcoord = textureLeftTop_ + textureSize_; // 右下

	vertexData_[3].texcoord = textureLeftTop_; // 左上
	vertexData_[4].texcoord = { textureLeftTop_.x + textureSize_.x,textureLeftTop_.y }; // 右上
	vertexData_[5].texcoord = textureLeftTop_ + textureSize_; // 右下
}

void Sprite::AdjustTextureSize()
{
	D3D12_RESOURCE_DESC resDesc = TextureManager::GetInstance()->GetTextureDesc(textureHundle_);

	size_ = { static_cast<float>(resDesc.Width),static_cast<float>(resDesc.Height) };
}

void Sprite::CreateVertexRes()
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

void Sprite::CreateMaterialRes()
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

void Sprite::CreateTranformRes()
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


