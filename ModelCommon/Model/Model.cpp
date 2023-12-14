#include "Model.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/DirectXCommon/DirectXCommon.h"
#include "ModelCommon/ModelData/ModelDataManager/ModelDataManager.h"

Model::Model(const std::string& fileName)
{

	ModelCommon* modelCommon = ModelCommon::GetInstance();

	ModelDataManager* modelManager = ModelDataManager::GetInstance();

	meshHundle_ = modelManager->LoadObj(fileName);

	textureHundle_ = modelManager->GetModelData(meshHundle_)->textureHundle_;

	materialResource_ = DirectXCommon::CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , 0 };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	instancingResource_ = DirectXCommon::CreateBufferResource(sizeof(TransformationMatrix));
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	*instancingData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4() };


	//平行光源用のリソースを作る。
	directionalLightResource_ = DirectXCommon::CreateBufferResource(sizeof(DirectionalLight));
	//データを書き込む
	directionalLightData_ = nullptr;
	//書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//書き込んでいく
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

	transforms_ = Transform();

	uvScale_ = { 1.0f,1.0f,1.0f };
	uvRotate_ = { 0.0f,0.0f,0.0f };
	uvPos_ = { 0.0f,0.0f,0.0f };

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

Model::Model(uint32_t meshHundle)
{
	ModelCommon* modelCommon = ModelCommon::GetInstance();

	ModelDataManager* modelManager = ModelDataManager::GetInstance();

	meshHundle_ = meshHundle;

	textureHundle_ = modelManager->GetModelData(meshHundle_)->textureHundle_;

	materialResource_ = DirectXCommon::CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , 0 };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	instancingResource_ = DirectXCommon::CreateBufferResource(sizeof(TransformationMatrix));
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));
	*instancingData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4() };


	//平行光源用のリソースを作る。
	directionalLightResource_ = DirectXCommon::CreateBufferResource(sizeof(DirectionalLight));
	//データを書き込む
	directionalLightData_ = nullptr;
	//書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//書き込んでいく
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;

	transforms_ = Transform();

	uvScale_ = { 1.0f,1.0f,1.0f };
	uvRotate_ = { 0.0f,0.0f,0.0f };
	uvPos_ = { 0.0f,0.0f,0.0f };

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

Model::~Model()
{
	instancingResource_->Release();
	materialResource_->Release();
	directionalLightResource_->Release();
}

void Model::Initialize()
{
	
}

void Model::Update()
{

	transforms_.UpdateMatrix();

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

void Model::Draw(const Matrix4x4& viewProjection)
{

	instancingData_->World = transforms_.worldMat_;
	instancingData_->WVP = transforms_.worldMat_ * viewProjection;
	materialData_->uvTransform = uvMatrix_;

	TextureManager* texManager = TextureManager::GetInstance();

	const ModelData* modelData = ModelDataManager::GetInstance()->GetModelData(meshHundle_);

	ModelCommon::GetInstance()->PreDraw();

	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	commandList->IASetVertexBuffers(0, 1, &modelData->mesh.vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(1, instancingResource_->GetGPUVirtualAddress());

	//平行光源CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(2, texManager->GetSRVGPUDescriptorHandle(textureHundle_));
	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(UINT(modelData->mesh.verteces.size()), 1, 0, 0);

}

void Model::SetMesh(uint32_t hundle)
{
	meshHundle_ = hundle;

	textureHundle_ = ModelDataManager::GetInstance()->GetTextureHundle(hundle);
}



