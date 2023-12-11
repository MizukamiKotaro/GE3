#include "Model.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/DirectXCommon/DirectXCommon.h"

Model::Model(const std::string& fileName)
{

	ModelCommon* modelCommon = ModelCommon::GetInstance();

	meshHundle_ = modelCommon->LoadObj(fileName);

	textureHundle_ = ModelCommon::GetInstance()->GetModelData(meshHundle_)->textureHundle_;

	materialResource_ = modelCommon->CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , 0 };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformationMatrixResource_ = modelCommon->CreateBufferResource(sizeof(TransformationMatrix));
	transformationMatrixData_ = nullptr;
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	*transformationMatrixData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4() };


	//平行光源用のリソースを作る。
	directionalLightResource_ = modelCommon->CreateBufferResource(sizeof(DirectionalLight));
	//データを書き込む
	directionalLightData_ = nullptr;
	//書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//書き込んでいく
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

	transform_ = Transform();

	uvScale_ = { 1.0f,1.0f,1.0f };
	uvRotate_ = { 0.0f,0.0f,0.0f };
	uvPos_ = { 0.0f,0.0f,0.0f };

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

Model::Model(uint32_t meshHundle)
{
	ModelCommon* modelCommon = ModelCommon::GetInstance();

	meshHundle_ = meshHundle;

	textureHundle_ = ModelCommon::GetInstance()->GetModelData(meshHundle_)->textureHundle_;

	materialResource_ = modelCommon->CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , 0 };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformationMatrixResource_ = modelCommon->CreateBufferResource(sizeof(TransformationMatrix));
	transformationMatrixData_ = nullptr;
	transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
	*transformationMatrixData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4() };


	//平行光源用のリソースを作る。
	directionalLightResource_ = modelCommon->CreateBufferResource(sizeof(DirectionalLight));
	//データを書き込む
	directionalLightData_ = nullptr;
	//書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//書き込んでいく
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

	transform_ = Transform();

	uvScale_ = { 1.0f,1.0f,1.0f };
	uvRotate_ = { 0.0f,0.0f,0.0f };
	uvPos_ = { 0.0f,0.0f,0.0f };

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

Model::~Model()
{
	transformationMatrixResource_->Release();
	materialResource_->Release();
	directionalLightResource_->Release();
}

void Model::Initialize()
{
	
}

void Model::Update()
{

	transform_.UpdateMatrix();

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

void Model::Draw(const Matrix4x4& viewProjection)
{

	transformationMatrixData_->World = transform_.worldMat_;
	transformationMatrixData_->WVP = transform_.worldMat_ * viewProjection;
	materialData_->uvTransform = uvMatrix_;

	TextureManager* texManager = TextureManager::GetInstance();

	ModelCommon::MeshData* modelData = ModelCommon::GetInstance()->GetModelData(meshHundle_);

	ModelCommon::GetInstance()->PreDraw();

	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	commandList->IASetVertexBuffers(0, 1, &modelData->vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());

	//平行光源CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(2, texManager->GetSRVGPUDescriptorHandle(textureHundle_));
	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(UINT(modelData->verteces.size()), 1, 0, 0);

}

void Model::SetMesh(uint32_t hundle)
{
	meshHundle_ = hundle;

	textureHundle_ = ModelCommon::GetInstance()->GetTextureHundle(hundle);
}



