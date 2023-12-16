#include "Sprite.h"

#include <cassert>
#include "SpriteCommon/SpriteCommon.h"
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"

Sprite::Sprite()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	//Sprite用の頂点リソースを作る
	vertexResource_ = SpriteCommon::GetInstance()->CreateBufferResource(sizeof(VertexData) * 6);
	//頂点バッファーを作成する
	//リソースの先頭アドレスから使う
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
	//1枚目の三角形
	vertexData_[0].vertexPos = { -0.5f,-0.5f,0.0f,1.0f }; // 左下
	vertexData_[0].texcoord = { 0.0f,1.0f };
	//vertexData_[0].normal = { 0.0f,0.0f, -1.0f };
	vertexData_[1].vertexPos = { -0.5f,0.5f,0.0f,1.0f }; // 左上
	vertexData_[1].texcoord = { 0.0f,0.0f };
	//vertexData_[1].normal = { 0.0f,0.0f, -1.0f };
	vertexData_[2].vertexPos = { 0.5f,-0.5f,0.0f,1.0f }; // 右下
	vertexData_[2].texcoord = { 1.0f,1.0f };
	//vertexData_[2].normal = { 0.0f,0.0f, -1.0f };
	
	vertexData_[3].vertexPos = { -0.5f,0.5f,0.0f,1.0f }; // 左上
	vertexData_[3].texcoord = { 0.0f,0.0f };
	//vertexData_[3].normal = { 0.0f,0.0f, -1.0f };
	vertexData_[4].vertexPos = { 0.5f,0.5f,0.0f,1.0f }; // 右上
	vertexData_[4].texcoord = { 1.0f,0.0f };
	//vertexData_[4].normal = { 0.0f,0.0f, -1.0f };
	vertexData_[5].vertexPos = { 0.5f,-0.5f,0.0f,1.0f }; // 右下
	vertexData_[5].texcoord = { 1.0f,1.0f };
	//vertexData_[5].normal = { 0.0f,0.0f, -1.0f };


	//マテリアル用のリソースを作る。今回はcolor1つ分を用意する
	materialResource_ = spriteCommon->CreateBufferResource(sizeof(Material));
	//マテリアルデータを書き込む
	//書き込むためのアドレスを取得\l
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	//今回は赤を書き込んでいる
	materialData_->color = { 1.0f,1.0f,1.0f,1.0f };
	//*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , false };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();

	//Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	instancingResource_ = spriteCommon->CreateBufferResource(sizeof(TransformationMatrix));
	//データを書き込む
	//書き込むためのアドレスを取得
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	//単位行列を書き込んでいく
	instancingData_->WVP = { Matrix4x4::MakeIdentity4x4() };
	//*transformationMatrixData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4() };



	// 汚いけど座標とかの初期化

	scale_ = { 1.0f,1.0f };
	rotate_ = 0.0f;
	pos_ = { 0.0f,0.0f };

	uvTranslate_ = {};
	uvScale_ = { 1.0f,1.0f };
	uvRotate_ = 0.0f;

	Update();
}

Sprite::~Sprite()
{
	vertexResource_->Release();
	instancingResource_->Release();
	materialResource_->Release();
}

void Sprite::Initialize()
{
	
}

void Sprite::Update()
{
	instancingData_->WVP = Matrix4x4::MakeAffinMatrix({ scale_.x,scale_.y }, { 0.0f,0.0f,rotate_ }, { pos_.x,pos_.y,0.0f });
	materialData_->uvTransform = Matrix4x4::MakeAffinMatrix({ uvScale_.x,uvScale_.y,0.0f }, { 0.0f,0.0f,uvRotate_ }, { uvTranslate_.x,uvTranslate_.y,0.0f });
}

void Sprite::Draw(BlendMode blendMode)
{
	TextureManager* texManager = TextureManager::GetInstance();

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	switch (blendMode)
	{
	case Sprite::BlendMode::kBlendModeNone:
		spriteCommon->PreDraw(SpriteCommon::BlendMode::kBlendModeNone);
		break;
	case Sprite::BlendMode::kBlendModeNormal:
		spriteCommon->PreDraw(SpriteCommon::BlendMode::kBlendModeNormal);
		break;
	case Sprite::BlendMode::kBlendModeAdd:
		spriteCommon->PreDraw(SpriteCommon::BlendMode::kBlendModeAdd);
		break;
	case Sprite::BlendMode::kBlendModeSubtract:
		spriteCommon->PreDraw(SpriteCommon::BlendMode::kBlendModeSubtract);
		break;
	case Sprite::BlendMode::kBlendModeMultiply:
		spriteCommon->PreDraw(SpriteCommon::BlendMode::kBlendModeMultiply);
		break;
	case Sprite::BlendMode::kBlendModeScreen:
		spriteCommon->PreDraw(SpriteCommon::BlendMode::kBlendModeScreen);
		break;
	default:
		spriteCommon->PreDraw(SpriteCommon::BlendMode::kBlendModeNormal);
		break;
	}

	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, instancingResource_->GetGPUVirtualAddress());
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


